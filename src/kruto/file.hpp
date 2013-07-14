#ifndef KRFILE_HPP
#define KRFILE_HPP

#include <stdio.h>

namespace kruto {

class File
{
public:
	static bool isRelative(const char *pFileName);
	static bool isTypeOf(const char *pFileName, const char *pType);
	static char *read(const char *pFileName, size_t *pSize = NULL);
};

}

#endif
