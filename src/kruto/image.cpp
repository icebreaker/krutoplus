#include "kruto/image.hpp"
#include "kruto/version.hpp"
#include "kruto/console.hpp"
#include "kruto/file.hpp"

#include <GL/gl.h>

namespace kruto {

Image::Image(void) : mSurface(NULL), mWidthF(0.0f), mHeightF(0.0f), mTextureId(0)
{
	// empty
}

Image::~Image(void)
{
	dispose();
}

const char *Image::fileName(void) const
{
	return mFileName;
}

bool Image::load(const char *pFileName)
{
	if(isLoaded())
		return true;

	if(File::isRelative(pFileName))
		return false;

	mSurface = IMG_Load(pFileName);
	if(mSurface == NULL)
	{
		mWidthF = 0.0f;
		mHeightF = 0.0f;
		Console::log("[image] Loading %s failed: %s", pFileName, IMG_GetError());
	}
	else
	{
		strncpy(mFileName, pFileName, 256);

		if(mTextureId == 0)
			glGenTextures(1, &mTextureId);

		glBindTexture(GL_TEXTURE_2D, mTextureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mSurface->w, mSurface->h, 0,
					 (mSurface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB), 
					 GL_UNSIGNED_BYTE, mSurface->pixels);

		glBindTexture(GL_TEXTURE_2D, 0);

		mWidthF = 1.0f / mSurface->w;
		mHeightF = 1.0f / mSurface->h;
	}

	return isLoaded();
}

void Image::dispose(void)
{
	if(mTextureId > 0)
	{
		glDeleteTextures(1, &mTextureId);
		mTextureId = 0;
	}

	if(mSurface != NULL)
	{
		SDL_FreeSurface(mSurface);
		mSurface = NULL;

		mWidthF = 0.0f;
		mHeightF = 0.0f;
	}
}

bool Image::isLoaded(void) const
{
	return mSurface != NULL;	 
}

int Image::width(void) const
{
	return mSurface != NULL ? mSurface->w : 0;
}

float Image::widthf(void) const
{
	return mWidthF;
}

int Image::height(void) const
{
	return mSurface != NULL ? mSurface->h : 0;
}

float Image::heightf(void) const
{
	return mHeightF;	 
}

int Image::bpp(void) const
{
	return mSurface->format->BytesPerPixel;
}

bool Image::hasAlpha(void) const
{
	return mSurface->format->BytesPerPixel == 4;
}

unsigned int Image::textureId(void) const
{
	return mTextureId;
}

bool Image::initialize(void)
{
	Console::log("[image] initialized");

	if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
	{
		Console::log("Failed to initialize SDL_image: %s", IMG_GetError());
		return false;
	}

	return true;
}

void Image::uninitialize(void)
{
	IMG_Quit();	
	Console::log("[image] uninitialized");
}

}
