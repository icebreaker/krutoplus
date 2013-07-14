#ifndef KRIMAGE_HPP
#define KRIMAGE_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

namespace kruto {

class Image
{
public:
	Image(void);
	~Image(void);

	const char *fileName(void) const;

	bool load(const char *pFileName);
	void dispose(void);

	bool isLoaded(void) const;

	int width(void) const;
	float widthf(void) const;
	
	int height(void) const;
	float heightf(void) const;		

	int bpp(void) const;

	bool hasAlpha(void) const;

	unsigned int textureId(void) const;

public:
	static bool initialize(void);
	static void uninitialize(void);

private:
	SDL_Surface *mSurface;
	float mWidthF;
	float mHeightF;
	unsigned int mTextureId;
	char mFileName[256];
};

}

#endif
