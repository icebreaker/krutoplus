#include "kruto/math.hpp"

#include <stdlib.h>
#include <time.h>

namespace kruto {

int	Math::random(const int pNumber)
{
	return rand() % pNumber;
}

void Math::randomize(void)
{
	srand(time(NULL));
}

}
