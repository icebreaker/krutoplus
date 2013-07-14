#ifndef KRCANVAS_HPP
#define KRCANVAS_HPP

#include <stdio.h>

#ifndef KR_CANVAS_WIDTH
	#define KR_CANVAS_WIDTH 1280
#endif

#ifndef KR_CANVAS_HEIGHT
	#define KR_CANVAS_HEIGHT 720
#endif

#ifndef KR_CANVAS_FULLSCREEN
	#define KR_CANVAS_FULLSCREEN false
#endif

namespace kruto {

class Image;
class ImageBatch;
class Canvas
{
public:
	enum BlendMode
	{
		NoBlend,
		AlphaBlend,
		AddBlend
	};

public:
	static bool setVideoMode(const unsigned int pWidth, const unsigned int pHeight, const bool pFullScreen=false);
	static bool isInitialized(void);

	static void begin(void);
	static void end(void);

	static void save(void);
	static void restore(void);
	static void scale(const float pX, const float pY);
	static void rotate(const float angle);
	static void translate(const float pX, const float pY);
	static void transform(const float pM11, const float pM12, const float pM21, const float pM22, const float pX, const float pY);
	static void setTransform(const float pM11, const float pM12, const float pM21, const float pM22, const float pX, const float pY);
	
	static void setColor(const char *pColor);
	static void setColor(const int pR, const int pG, const int pB);
	static void setColor(const int pR, const int pG, const int pB, const float pA);

	static void setClearColor(const char *pColor);
	static void setClearColor(const int pR, const int pG, const int pB);
	static void setClearColor(const int pR, const int pG, const int pB, const float pA);

	static void setLineWidth(const float pW);

	static void fillRect(const float pX, const float pY, const float pW, const float pH);
	static void strokeRect(const float pX, const float pY, const float pW, const float pH);
	static void clearRect(const float pX, const float pY, const float pW, const float pH);

	static void beginPolygon(const bool pFill=true);
	static void drawPoint(const float pX, const float pY);
	static void endPolygon(void);

	static void drawImage(const Image *pImage, const float pX, const float pY);
	static void drawImage(const Image *pImage, const float pX, const float pY, const float pW, const float pH);
	static void drawImage(const Image *pImage, const float pSX, const float pSY, const float pSW, const float pSH, const float pX, const float pY, const float pW, const float pH);

	static void drawImageBatch(const ImageBatch *pImageBatch);

	static void setBlendMode(const BlendMode pBlendMode);
};

}

#endif
