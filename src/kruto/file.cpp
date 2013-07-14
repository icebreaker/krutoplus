#include "kruto/file.hpp"
#include "kruto/console.hpp"

#include <string.h>

namespace kruto {

bool File::isRelative(const char *pFileName)
{
	if(pFileName == NULL || *pFileName == '\0')
		return true;
	
	if(*pFileName == '/' || *pFileName == '\\' || strstr(pFileName, "..") || strstr(pFileName, "::"))
	{
		Console::log("[file] cannot access %s (outside of sandbox)", pFileName);
		return true;
	}

	return false;
}

bool File::isTypeOf(const char *pFileName, const char *pType)
{
	int lf = strlen(pFileName);
	int lt = strlen(pType);

	const char *sf = pFileName + lf;	
	const char *st = pType + lt;

	if(lf < lt)
		return false;

	while(lt--)
	{
		if(*st-- != *sf--)
			return false;
	}

	return true;
}

char *File::read(const char *pFileName, size_t *pSize)
{
	if(File::isRelative(pFileName))
		return NULL;

	FILE *fp = fopen(pFileName, "r");
	if(fp == NULL)
		return NULL;

	size_t offset = fseek(fp, 0L, SEEK_END);
	size_t size = ftell(fp);
	fseek(fp, offset, SEEK_SET);

	char *buffer = new char[size+1];
	fread(buffer, sizeof(char), size, fp);
	buffer[size] = '\0';

	fclose(fp);

	if(pSize != NULL)
		*pSize = size;

	return buffer;
}

}
