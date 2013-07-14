#include "kruto/application.hpp"

#include <SDL/SDL.h>

namespace kruto {

Application::Application(const int pArgc, char **pArgv) : 
	mArgc(pArgc), 
	mArgv(pArgv),
	mWidth(KR_CANVAS_WIDTH),
	mHeight(KR_CANVAS_HEIGHT),
	mFullScreen(KR_CANVAS_FULLSCREEN),
	mRunning(true)
{
	Console::log("[app] created");

	for(int i=0; i<512; i++)
		mKeys[i] = false;

	for(int i=0; i<3; i++)
		mButtons[i] = false;

	Math::randomize();
}

Application::~Application(void)
{
	Console::log("[app] destroyed");
}

bool Application::initialize(void)
{
	Console::log("[app] initialized");

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		Console::log("[app] Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	if(!Canvas::setVideoMode(mWidth, mHeight, mFullScreen))
	{
		Console::log("[app] Failed to initialize Canvas");
		return false;
	}

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
//	SDL_WM_GrabInput(SDL_GRAB_ON);

	setTitle("Kruto+");

	if(!Image::initialize())
		return false;

#ifndef KR_NO_AUDIO
	if(!Audio::initialize())
		return false;
#endif

	return true;
}

void Application::uninitialize(void)
{
#ifndef KR_NO_AUDIO
	Audio::uninitialize();
#endif

	Image::uninitialize();

	SDL_Quit();
	Console::log("[app] unintialized");
}

void Application::update(const unsigned int pDelta)
{
	KR_UNUSED(pDelta);
	// empty
}
	
void Application::draw(void)
{
	// empty
}
	
void Application::onKeyDown(const KeyEvent *pKeyEvent)
{
	KR_ASSERT(pKeyEvent->key() >= 0 && pKeyEvent->key() < 512);
	mKeys[pKeyEvent->key()] = true;
}

void Application::onKeyUp(const KeyEvent *pKeyEvent)
{
	KR_ASSERT(pKeyEvent->key() >= 0 && pKeyEvent->key() < 512);
	mKeys[pKeyEvent->key()] = false;
}

void Application::onMouseDown(const MouseEvent *pMouseEvent)
{
	KR_ASSERT(pMouseEvent->key() >= 0 && pMouseEvent->button() < 3);

	mButtons[pMouseEvent->button()] = true;
	
	mMouseX = pMouseEvent->x();
	mMouseY = pMouseEvent->y();
}

void Application::onMouseUp(const MouseEvent *pMouseEvent)
{
	KR_ASSERT(pMouseEvent->key() >= 0 && pMouseEvent->button() < 3);

	mButtons[pMouseEvent->button()] = false;

	mMouseX = pMouseEvent->x();
	mMouseY = pMouseEvent->y();
}

void Application::onMouseMove(const MouseEvent *pMouseEvent)
{
	mMouseX = pMouseEvent->x();
	mMouseY = pMouseEvent->y();
}
	
void Application::processEvents(void)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
			{
				KeyEvent e(event.key.keysym.sym, event.key.keysym.mod);
				onKeyDown(&e);
			}
			break;

			case SDL_KEYUP:
			{
				KeyEvent e(event.key.keysym.sym, event.key.keysym.mod);
				onKeyUp(&e);
			}
			break;

			case SDL_MOUSEBUTTONDOWN:
			{
				MouseEvent e(event.button.button, event.button.x, event.button.y);
				onMouseDown(&e);
			}
			break;

			case SDL_MOUSEBUTTONUP:
			{
				MouseEvent e(event.button.button, event.button.x, event.button.y);
				onMouseUp(&e);
			}
			break;

			case SDL_MOUSEMOTION:
			{
				MouseEvent e(MouseButtons::None, event.motion.x, event.motion.y);
				onMouseMove(&e);
			}
			break;

			case SDL_QUIT:
				mRunning = false;
				break;
		}
	}
}

int Application::exec(void)
{
	unsigned int last = ticks();

	if(!initialize() || !Canvas::isInitialized())
	{
		uninitialize();
		return EXIT_FAILURE;
	}

	while(mRunning)
	{
		unsigned int now = ticks();
		unsigned int dt = now - last;
		last = now;

		draw();
		update(dt);
		processEvents();
	}

	uninitialize();
	return EXIT_SUCCESS;
}

void Application::setTitle(const char *pTitle)
{
	SDL_WM_SetCaption(pTitle, pTitle);
}

const char *Application::title(void) const
{
	char *title;
	SDL_WM_GetCaption(&title, NULL);
	return title;
}

int Application::argc(void) const
{
	return mArgc;	 
}

char **Application::argv(void) const
{
	return mArgv;	 
}

const char *Application::argv(const unsigned int pIndex)
{
	return mArgv[pIndex];	 
}

unsigned int Application::width(void) const
{
	return mWidth;
}

unsigned int Application::height(void) const
{
	return mHeight;	 
}

void Application::setSize(const int pWidth, const int pHeight)
{
	mWidth = pWidth;
	mHeight = pHeight;

	if(Canvas::isInitialized())
	{
		if(!Canvas::setVideoMode(mWidth, mHeight, mFullScreen))
			Console::log("[app] Failed to initialize Canvas");
	}
}

void Application::setFullScreen(const bool pFullScreen)
{
	mFullScreen = pFullScreen;

	if(Canvas::isInitialized())
	{
		if(!Canvas::setVideoMode(mWidth, mHeight, mFullScreen))
			Console::log("[app] Failed to initialize Canvas");
	}
}

unsigned int Application::ticks(void) const
{
	return SDL_GetTicks();
}

bool Application::isKeyDown(const Keys::Key pKey) const
{
	return mKeys[pKey];
}

bool Application::isMouseDown(const MouseButtons::Button pButton) const
{
	return mButtons[pButton];
}

int Application::mouseX(void) const
{
	return mMouseX;
}

int Application::mouseY(void) const
{
	return mMouseY;	 
}

void Application::quit(void)
{
	mRunning = false;	 
}

}
