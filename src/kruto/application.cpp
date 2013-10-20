#include "kruto/application.hpp"

#include <SDL/SDL.h>

namespace kruto {

Application::Application(const int pArgc, 
		char **pArgv,
		const char *pTitle,
		const int pWidth,
		const int pHeight,
		const bool pFullScreen,
		const bool pAudio) :
	mArgc(pArgc), 
	mArgv(pArgv),
	mWidth(pWidth),
	mHeight(pHeight),
	mFullScreen(pFullScreen),
	mAudio(pAudio),
	mRunning(true)
{
	Console::log("[app] created");

	setTitle(pTitle);

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

	Uint32 flags = SDL_INIT_VIDEO;

#ifndef KR_NO_AUDIO
	if(mAudio)
		flags |= SDL_INIT_AUDIO;
#endif

	if(SDL_Init(flags) < 0)
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

	SDL_WM_SetCaption(mTitle, mTitle);

	if(!Image::initialize())
		return false;

#ifndef KR_NO_AUDIO
	if((SDL_getenv("KR_NO_AUDIO") == NULL && mAudio) && !Audio::initialize())
		return false;
#endif

	return true;
}

void Application::uninitialize(void)
{
#ifndef KR_NO_AUDIO
	if(SDL_getenv("KR_NO_AUDIO") == NULL && mAudio)
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
	unsigned int now = 0;
	unsigned int last = 0;
	unsigned int dt = 0;
	unsigned int frames = 0;
	unsigned int framesLastTime = 0;

	if(!initialize() || !Canvas::isInitialized())
	{
		uninitialize();
		return EXIT_FAILURE;
	}

	now = ticks();
	last = now;
	framesLastTime = last;

	while(mRunning)
	{
		draw();
		update(dt);
		processEvents();

		now = ticks();
		dt = now - last;
		last = now;

		frames++;
		if(now > framesLastTime + 1000)
		{
			mFps = frames;
			framesLastTime = now;
			frames = 0;
		}
	}

	uninitialize();
	return EXIT_SUCCESS;
}

void Application::setTitle(const char *pTitle)
{
	if(pTitle != NULL)
	{
		strncpy(mTitle, pTitle, 255);
		mTitle[255] = '\0';

		if(Canvas::isInitialized())
			SDL_WM_SetCaption(mTitle, mTitle);
	}
}

const char *Application::title(void) const
{
	return mTitle;
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

int Application::fps(void) const
{
	return mFps;
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
