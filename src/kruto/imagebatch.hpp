#ifndef KRIMAGEBATCH_HPP
#define KRIMAGEBATCH_HPP

namespace kruto {

struct Vertex;
class Image;
class ImageBatch
{
	friend class Canvas;

public:
	ImageBatch(void);
	virtual ~ImageBatch(void);

	void setImage(const Image *pImage);
	void setImage(const Image *pImage, 
				  const float pSX, 
				  const float pSY,
				  const float pSW,
				  const float pSH);

	const Image *image(void) const;

	void reserve(const int pSize);
	void free(void);

	int size(void) const;
	Vertex *ptr(void) const;

	void set(const unsigned int pIndex, const float pX, const float pY, const float pW, const float pH);
	void set(const unsigned int pIndex, const float pX, const float pY);

	void setSize(const unsigned int pIndex, const float pW, const float pH);
	void setAlpha(const unsigned int pIndex, const float pA);

	void setAlpha(const float pA);

	void translate(const unsigned pIndex, const float pX, const float pY);
//	void scale(const unsigned pIndex, const float pW, const float pH);

protected:
	void initialize(void);

private:
	const Image *mImage;
	
	Vertex *mVertices;
	int mVertexCount;

	int mSize;

	float mSX;
	float mSY;
	float mSW;
	float mSH;

	float mA;
};

}

#endif
