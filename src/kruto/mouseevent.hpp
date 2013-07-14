#ifndef KRMOUSEEVENT_HPP
#define KRMOUSEEVENT_HPP

#include <SDL/SDL.h>

namespace kruto {

class MouseEvent
{
public:
	MouseEvent(const int pButton, const int pX, const int pY);
	virtual ~MouseEvent(void);

	int button(void) const;

	int x(void) const;
	int y(void) const;

protected:
	MouseEvent(const MouseEvent &pOther);
	MouseEvent &operator=(const MouseEvent &pOther);

private:
	int mButton;
	int mX;
	int mY;
};

namespace MouseButtons
{
	enum Button
	{
		None	= 0,
		Left	= SDL_BUTTON_LEFT,
		Middle	= SDL_BUTTON_MIDDLE,
		Right	= SDL_BUTTON_RIGHT
	};
}

}

#endif
