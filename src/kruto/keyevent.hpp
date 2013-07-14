#ifndef KRKEYEVENT_HPP
#define KRKEYEVENT_HPP

#include <SDL/SDL.h>

namespace kruto {

class KeyEvent
{
public:
	KeyEvent(const int pKey, const int pMods);
	virtual ~KeyEvent(void);

	int key(void) const;

	bool alt(void) const;
	bool shift(void) const;
	bool control(void) const;

protected:
	KeyEvent(const KeyEvent &pOther);
	KeyEvent &operator=(const KeyEvent &pOther);

private:
	int mKey;
	int mMods;
};

namespace Keys 
{
	enum Key
	{
		Escape	= SDLK_ESCAPE,
		Enter	= SDLK_RETURN,
		Space	= SDLK_SPACE,
		Left	= SDLK_LEFT,
		Right	= SDLK_RIGHT,
		Up		= SDLK_UP,
		Down	= SDLK_DOWN
	};
}

}

#endif
