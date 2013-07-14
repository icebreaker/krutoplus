#include "kruto/console.hpp"
#include <stdio.h>
#include <stdarg.h>

namespace kruto {

void Console::log(const char *pFormat, ...)
{
	char buffer[16384] = {0,};

	va_list args;
	va_start(args, pFormat);
	vsnprintf(buffer, 16384, pFormat, args);
	va_end(args);

	printf("%s\n", buffer);
}

}
