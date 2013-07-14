#ifndef KRAPPLICATION_HPP
#define KRAPPLICATION_HPP

#include "kruto/version.hpp"
#include "kruto/console.hpp"
#include "kruto/math.hpp"
#include "kruto/file.hpp"
#include "kruto/canvas.hpp"
#include "kruto/timer.hpp"
#include "kruto/image.hpp"
#include "kruto/imagebatch.hpp"
#include "kruto/vertex.hpp"
#include "kruto/audio.hpp"
#include "kruto/keyevent.hpp"
#include "kruto/mouseevent.hpp"

namespace kruto {

class Application
{
public:
	Application(const int pArgc, char **pArgv);
	virtual ~Application(void);

	virtual bool initialize(void);
	virtual void uninitialize(void);

	virtual void update(const unsigned int pDelta);
	virtual void draw(void);

	virtual void onKeyDown(const KeyEvent *pKeyEvent);
	virtual void onKeyUp(const KeyEvent *pKeyEvent);

	virtual void onMouseDown(const MouseEvent *pMouseEvent);
	virtual void onMouseUp(const MouseEvent *pMouseEvent);
	virtual void onMouseMove(const MouseEvent *pMouseEvent);

	void processEvents(void);

	int exec(void);

	void setTitle(const char *pTitle);
	const char *title(void) const;

	int argc(void) const;
	char **argv(void) const;
	const char *argv(const unsigned int pIndex);

	unsigned int width(void) const;
	unsigned int height(void) const;

	void setSize(const int pWidth, const int pHeight);
	void setFullScreen(const bool pFullScreen);

	unsigned int ticks(void) const;

	bool isKeyDown(const Keys::Key pKey) const;
	bool isMouseDown(const MouseButtons::Button pButton) const;

	int mouseX(void) const;
	int mouseY(void) const;

	void quit(void);

private:
	int mArgc;
	char **mArgv;
	int mWidth;
	int mHeight;
	bool mFullScreen;
	bool mRunning;
	bool mKeys[512];
	bool mButtons[3];
	int mMouseX;
	int mMouseY;
};

}

#endif
