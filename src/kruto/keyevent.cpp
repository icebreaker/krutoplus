#include "keyevent.hpp"

namespace kruto {

KeyEvent::KeyEvent(const int pKey, const int pMods) : mKey(pKey), mMods(pMods)
{
	// empty
}

KeyEvent::~KeyEvent(void)
{
	// empty
}

int KeyEvent::key(void) const
{
	return mKey;
}

bool KeyEvent::alt(void) const
{
	return ((mMods & KMOD_LALT) || (mMods & KMOD_RALT));
}

bool KeyEvent::shift(void) const
{
	return ((mMods & KMOD_LSHIFT) || (mMods & KMOD_RSHIFT));
}

bool KeyEvent::control(void) const
{
	return ((mMods & KMOD_LCTRL) || (mMods & KMOD_RCTRL));
}

}
