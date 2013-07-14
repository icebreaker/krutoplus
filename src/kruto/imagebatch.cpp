#include "kruto/imagebatch.hpp"
#include "kruto/image.hpp"
#include "kruto/vertex.hpp"

namespace kruto {

ImageBatch::ImageBatch(void) : 
	mImage(NULL), 
	mVertices(NULL),
	mVertexCount(0),
	mSize(0),
	mSX(0.0f),
	mSY(0.0f),
	mSW(0.0f),
	mSH(0.0f)
{
	mColor[0] = 1.0f;
	mColor[1] = 1.0f;
	mColor[2] = 1.0f;
	mColor[3] = 1.0f;
}

ImageBatch::~ImageBatch(void)
{
	free();
}

void ImageBatch::setImage(const Image *pImage)
{
	mImage = pImage;

	mSX = 0.0f;
	mSY = 0.0f;
	mSW = mImage->width();
	mSH = mImage->height();
}

void ImageBatch::setImage(const Image *pImage, const float pSX, const float pSY, const float pSW, const float pSH)
{
	mImage = pImage;

	mSX = pSX;
	mSY = pSY;
	mSW = pSW;
	mSH = pSH;
}

const Image *ImageBatch::image(void) const
{
	return mImage;
}

void ImageBatch::reserve(const int pSize)
{
	free();

	mSize = pSize;
	mVertexCount = mSize * 4;
	mVertices = new Vertex[mVertexCount];

	initialize();
}

void ImageBatch::free(void)
{
	if(mVertices != NULL)
	{
		delete [] mVertices;
		mVertices = NULL;

		mVertexCount = 0;
		mSize = 0;
	}
}

int ImageBatch::size(void) const
{
	return mSize;	 
}

Vertex *ImageBatch::ptr(void) const
{
	return mVertices;
}

void ImageBatch::get(const unsigned int pIndex, float *pX, float *pY) const
{
	Vertex *v = mVertices + (pIndex * 4);
	*pX = v->mPos[0];
	*pY = v->mPos[1];
}

void ImageBatch::get(const unsigned int pIndex, float *pX, float *pY, float *pA) const
{
	Vertex *v = mVertices + (pIndex * 4);
	*pX = v->mPos[0];
	*pY = v->mPos[1];
	*pA = v->mColor[3];
}

float ImageBatch::getAlpha(const unsigned int pIndex) const
{
	Vertex *v = mVertices + (pIndex * 4);
	return v->mColor[3];
}

void ImageBatch::set(const unsigned int pIndex, const float pX, const float pY, const float pW, const float pH)
{
	Vertex *v = mVertices + (pIndex * 4);
	v->mPos[0] = pX;
	v->mPos[1] = pY;

	v++;
	v->mPos[0] = pX + pW;
	v->mPos[1] = pY;

	v++;
	v->mPos[0] = pX + pW;
	v->mPos[1] = pY + pH;

	v++;
	v->mPos[0] = pX;
	v->mPos[1] = pY + pH;
}

void ImageBatch::set(const unsigned int pIndex, const float pX, const float pY)
{
	Vertex *v = mVertices + (pIndex * 4);
	v->mPos[0] = pX;
	v->mPos[1] = pY;

	v++;
	v->mPos[0] = pX + mSW;
	v->mPos[1] = pY;

	v++;
	v->mPos[0] = pX + mSW;
	v->mPos[1] = pY + mSH;

	v++;
	v->mPos[0] = pX;
	v->mPos[1] = pY + mSH;
}

void ImageBatch::setSize(const unsigned int pIndex, const float pW, const float pH)
{
	Vertex *v = mVertices + (pIndex * 4);
	float x = v->mPos[0];
	float y = v->mPos[1];

	v++;
	v->mPos[0] = x + pW;

	v++;
	v->mPos[0] = x + pW;
	v->mPos[1] = y + pH;

	v++;
	v->mPos[1] = y + pH;
}

void ImageBatch::setColor(const unsigned int pIndex, const int pR, const int pG, const int pB)
{
	Vertex *v = mVertices + (pIndex * 4);
	float m = 1.0f / 255.0f;
	setColor(v, pR * m, pG * m, pB * m);
}

void ImageBatch::setColor(const unsigned int pIndex, const int pR, const int pG, const int pB, const float pA)
{
	Vertex *v = mVertices + (pIndex * 4);
	float m = 1.0f / 255.0f;
	setColor(v, pR * m, pG * m, pB * m, pA);
}

void ImageBatch::setGlobalColor(const int pR, const int pG, const int pB)
{
	float m = 1.0f / 255.0f;

	mColor[0] = pR * m;
	mColor[1] = pG * m;
	mColor[2] = pB * m;

	if(mVertices == NULL)
		return;

	for(int i=0; i<mSize; i++)
		setColor(i, pR, pG, pB);
}

void ImageBatch::setGlobalColor(const int pR, const int pG, const int pB, const float pA)
{
	float m = 1.0f / 255.0f;

	mColor[0] = pR * m;
	mColor[1] = pG * m;
	mColor[2] = pB * m;
	mColor[3] = pA;

	if(mVertices == NULL)
		return;

	for(int i=0; i<mSize; i++)
		setColor(i, pR, pG, pB, pA);
}

void ImageBatch::setAlpha(const unsigned int pIndex, const float pA)
{
	Vertex *v = mVertices + (pIndex * 4);
	v->mColor[3] = pA;

	v++;
	v->mColor[3] = pA;

	v++;
	v->mColor[3] = pA;

	v++;
	v->mColor[3] = pA;
}

void ImageBatch::setGlobalAlpha(const float pA)
{
	mColor[3] = pA;

	if(mVertices == NULL)
		return;

	for(int i=0; i<mSize; i++)
		setAlpha(i, pA);
}

void ImageBatch::translate(const unsigned int pIndex, const float pX, const float pY)
{
	Vertex *v = mVertices + (pIndex * 4);
	v->mPos[0] += pX;
	v->mPos[1] += pY;

	v++;
	v->mPos[0] += pX;
	v->mPos[1] += pY;

	v++;
	v->mPos[0] += pX;
	v->mPos[1] += pY;

	v++;
	v->mPos[0] += pX;
	v->mPos[1] += pY;
}

void ImageBatch::scale(const unsigned pIndex, const float pW, const float pH)
{
	Vertex *v = mVertices + (pIndex * 4);
	v->mPos[0] -= pW;
	v->mPos[1] -= pH;

	v++;
	v->mPos[0] += pW;
	v->mPos[1] -= pH;

	v++;
	v->mPos[0] += pW;
	v->mPos[1] += pH;

	v++;
	v->mPos[0] -= pW;
	v->mPos[1] += pH;
}

void ImageBatch::initialize(void)
{
	float sx = 0.0f, sy = 0.0f, sw = 1.0f, sh = 1.0f;

	if(mSX > 0.0f || mSY > 0.0f || mSW < mImage->width() || mSH < mImage->height())
	{
		sx = mSX * mImage->widthf();
		sy = mSY * mImage->heightf();

		sw = mSW * mImage->widthf();
		sh = mSH * mImage->heightf();
	}

	for(int i=0; i<mSize; i++)
	{
		Vertex *v = mVertices + (i * 4);
		v->mPos[0] = 0.0f;
		v->mPos[1] = 0.0f;

		v->mCoords[0] = sx;
		v->mCoords[1] = sy;

		memcpy(v->mColor, mColor, sizeof(mColor));

		v++;
		v->mPos[0] = 0.0f;
		v->mPos[1] = 0.0f;

		v->mCoords[0] = sx + sw;
		v->mCoords[1] = sy;

		memcpy(v->mColor, mColor, sizeof(mColor));

		v++;
		v->mPos[0] = 0.0f;
		v->mPos[1] = 0.0f;

		v->mCoords[0] = sx + sw;
		v->mCoords[1] = sy + sh;

		memcpy(v->mColor, mColor, sizeof(mColor));

		v++;
		v->mPos[0] = 0.0f;
		v->mPos[1] = 0.0f;

		v->mCoords[0] = sx;
		v->mCoords[1] = sy + sh;

		memcpy(v->mColor, mColor, sizeof(mColor));
	}
}

void ImageBatch::setColor(Vertex *pV, const float pR, const float pG, const float pB)
{
	pV->mColor[0] = pR;
	pV->mColor[1] = pG;
	pV->mColor[2] = pB;

	pV++;
	pV->mColor[0] = pR;
	pV->mColor[1] = pG;
	pV->mColor[2] = pB;

	pV++;
	pV->mColor[0] = pR;
	pV->mColor[1] = pG;
	pV->mColor[2] = pB;

	pV++;
	pV->mColor[0] = pR;
	pV->mColor[1] = pG;
	pV->mColor[2] = pB;
}

void ImageBatch::setColor(Vertex *pV, const float pR, const float pG, const float pB, const float pA)
{
	pV->mColor[0] = pR;
	pV->mColor[1] = pG;
	pV->mColor[2] = pB;
	pV->mColor[3] = pA;

	pV++;
	pV->mColor[0] = pR;
	pV->mColor[1] = pG;
	pV->mColor[2] = pB;
	pV->mColor[3] = pA;

	pV++;
	pV->mColor[0] = pR;
	pV->mColor[1] = pG;
	pV->mColor[2] = pB;
	pV->mColor[3] = pA;

	pV++;
	pV->mColor[0] = pR;
	pV->mColor[1] = pG;
	pV->mColor[2] = pB;
	pV->mColor[3] = pA;
}

}
