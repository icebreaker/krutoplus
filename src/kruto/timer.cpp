#include "kruto/timer.hpp"
#include "kruto/console.hpp"

#include <SDL/SDL.h>

namespace kruto {

void Timer::delay(const unsigned int pDelay)
{
	SDL_Delay(pDelay);	 
}

}
