#include "mouseevent.hpp"

namespace kruto {

MouseEvent::MouseEvent(const int pButton, const int pX, const int pY) : 
	mButton(pButton), 
	mX(pX), 
	mY(pY)
{
	// empty
}

MouseEvent::~MouseEvent(void)
{
	// empty
}

int MouseEvent::button(void) const
{
	return mButton;
}

int MouseEvent::x(void) const
{
	return mX;	 
}

int MouseEvent::y(void) const
{
	return mY;
}

}
