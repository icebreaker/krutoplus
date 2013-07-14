#include "kruto/canvas.hpp"
#include "kruto/console.hpp"
#include "kruto/image.hpp"
#include "kruto/imagebatch.hpp"
#include "kruto/vertex.hpp"

#include <math.h>

#include <SDL/SDL.h>
#include <GL/gl.h>

#define KR_CENTER_WINDOW

//#ifndef KR_VSYNC
//	#define KR_VSYNC 1
//#endif

#ifndef KR_CANVAS_COLOR
	#define KR_CANVAS_COLOR 0.0f
#endif

#ifndef KR_MSAA
	#define KR_MSAA 4
#endif

namespace kruto {

bool Canvas::setVideoMode(const unsigned int pWidth, const unsigned int pHeight, const bool pFullScreen)
{
	int flags = SDL_OPENGL;

	if(pFullScreen)
		flags |= SDL_FULLSCREEN;

#ifdef KR_CENTER_WINDOW
	SDL_putenv(const_cast<char *>("SDL_VIDEO_WINDOW_POS"));
	SDL_putenv(const_cast<char *>("SDL_VIDEO_CENTERED=1"));
#endif

#if defined(KR_MSAA) && KR_MSAA > 0
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, KR_MSAA);
#endif

#ifdef KR_VSYNC
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, KR_VSYNC);
#endif

	if(SDL_SetVideoMode(pWidth, pHeight, 0, flags) == NULL)
	{
		Console::log("[canvas] Failed to set videomode %dx%d: %s", pWidth, pHeight, SDL_GetError());
		return false;
	}
	else
	{
		Console::log("[canvas] size %dx%d", pWidth, pHeight);	 
	}

#if defined(KR_MSAA) && KR_MSAA > 0
	glEnable(GL_MULTISAMPLE);
#endif

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	 
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, pWidth, pHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, pWidth, pHeight, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glFinish();
	SDL_GL_SwapBuffers();

	return true;
}

bool Canvas::isInitialized(void)
{
	return SDL_GetVideoSurface() != NULL;
}

void Canvas::begin(void)
{
#ifdef KR_QUIRKS_MODE
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
#endif
}

void Canvas::end(void)
{
	glFinish();
	SDL_GL_SwapBuffers();
}

void Canvas::save(void)
{
	glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT | GL_TEXTURE_BIT);
	glPushMatrix();
}

void Canvas::restore(void)
{
	glPopMatrix();
	glPopAttrib(); 
}

void Canvas::scale(const float pX, const float pY)
{
	glScalef(pX, pY, 0.0f);
}

void Canvas::rotate(const float pAngle)
{
	glRotatef(pAngle * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
}

void Canvas::translate(const float pX, const float pY)
{
	glTranslatef(pX, pY, 0.0f);
}

void Canvas::transform(const float pM11, const float pM12, const float pM21, const float pM22, const float pX, const float pY)
{
	const float m[16] =
	{
		pM11, pM21,   pX, 0.0f,
		pM12, pM22,   pY, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	glMultMatrixf(m);
}

void Canvas::setTransform(const float pM11, const float pM12, const float pM21, const float pM22, const float pX, const float pY)
{
	const float m[16] = 
	{
		pM11, pM21,   pX, 0.0f,
		pM12, pM22,   pY, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	glLoadMatrixf(m);
}

// TODO: we can make this better :)
#define KR_COLOR(c, i) reinterpret_cast<unsigned char *>(&c)[i]
#define KR_CR(c) KR_COLOR(c, 2)
#define KR_CG(c) KR_COLOR(c, 1)
#define KR_CB(c) KR_COLOR(c, 0)
#define KR_C1(c) \
do \
{\
	unsigned int color = strtoul(c + 1, NULL, 16); \
	glColor3ub(KR_CR(color), KR_CG(color), KR_CB(color)); \
} while(0)
#define KR_C2(c) \
do \
{ \
	int r = 0, g = 0, b = 0; float a = 0.0f; \
	if(sscanf(c, "rgba(%d,%d,%d,%f)", &r, &g, &b, &a) == 4) \
		glColor4ub(r, g, b, a * 255.0f); \
} while(0)

void Canvas::setColor(const char *pColor)
{
	if(*pColor == '#') // #rgb
		KR_C1(pColor);
	else if(*pColor == 'r') // rgba(r,g,b,a)
		KR_C2(pColor);
}

void Canvas::setColor(const int pR, const int pG, const int pB)
{
	glColor3ub(pR, pG, pB);	 
}

void Canvas::setColor(const int pR, const int pG, const int pB, const float pA)
{
	glColor4ub(pR, pG, pB, pA * 255.0f); 
}

void Canvas::setClearColor(const char *pColor)
{
	float r = KR_CANVAS_COLOR, g = KR_CANVAS_COLOR, b = KR_CANVAS_COLOR, a = 1.0f;

	if(*pColor == '#') // #rgb
	{
		unsigned int color = strtoul(pColor+1, NULL, 16);
		r = KR_CR(color) / 255.0f;
		g = KR_CG(color) / 255.0f;
		b = KR_CB(color) / 255.0f;
	}
	else if(*pColor == 'r') // rgba(r,g,b,a)
	{
		int rr = 0, gg = 0, bb = 0;
		if(sscanf(pColor, "rgba(%d,%d,%d,%f)", &rr, &gg, &bb, &a) == 4)
		{
			r = rr / 255.0f;
			g = gg / 255.0f;
			b = bb / 255.0f;
		}
	}

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Canvas::setClearColor(const int pR, const int pG, const int pB)
{
	glClearColor(pR / 255.0f, pG / 255.0f, pB / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Canvas::setClearColor(const int pR, const int pG, const int pB, const float pA)
{
	glClearColor(pR / 255.0f, pG / 255.0f, pB / 255.0f, pA);
	glClear(GL_COLOR_BUFFER_BIT);
}

#undef KR_COLOR
#undef KR_CR
#undef KR_CG
#undef KR_CB
#undef KR_C1
#undef KR_C2

void Canvas::setLineWidth(const float pW)
{
	glLineWidth(pW);
}

#define KR_RECT(x, y, w, h) \
	glVertex2d(x,   y); \
	glVertex2d(x+w, y); \
	glVertex2d(x+w, y+h); \
	glVertex2d(x,   y+h)

void Canvas::fillRect(const float pX, const float pY, const float pW, const float pH)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_FAN);
		KR_RECT(pX, pY, pW, pH);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void Canvas::strokeRect(const float pX, const float pY, const float pW, const float pH)
{
	int lw = 0; float hlw = 0.0f;
	glGetIntegerv(GL_LINE_WIDTH, &lw);
	hlw = lw / 2.0f;

	glDisable(GL_TEXTURE_2D);
	
	glBegin(GL_TRIANGLE_FAN);
		KR_RECT(pX, pY-hlw, pW, lw);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		KR_RECT(pX+pW, pY-hlw, lw, pH);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		KR_RECT(pX, pY+pH-hlw, pW+lw, lw);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		KR_RECT(pX, pY-hlw, lw, pH);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}
#undef KR_RECT

void Canvas::clearRect(const float pX, const float pY, const float pW, const float pH)
{
	int vp[4] = {0,};
	glGetIntegerv(GL_VIEWPORT, vp);

	glEnable(GL_SCISSOR_TEST);
	glScissor(pX, vp[3] - pH - pY, pW, pH); // invert Y axis
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}

void Canvas::beginPolygon(const bool pFill)
{
	if(pFill)
		glBegin(GL_POLYGON);
	else
		glBegin(GL_LINE_LOOP);
}

void Canvas::drawPoint(const float pX, const float pY)
{
	glVertex2d(pX, pY);
}

void Canvas::endPolygon(void)
{
	glEnd();	 
}

void Canvas::drawImage(const Image *pImage, const float pX, const float pY)
{
	int w = pImage->width(), h = pImage->height();

	glBindTexture(GL_TEXTURE_2D, pImage->textureId());
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2i(0, 0); glVertex2d(pX,   pY  );
		glTexCoord2i(1, 0); glVertex2d(pX+w, pY  );
		glTexCoord2i(1, 1); glVertex2d(pX+w, pY+h);
		glTexCoord2i(0, 1); glVertex2d(pX,   pY+h);
	glEnd(); 
}

void Canvas::drawImage(const Image *pImage, const float pX, const float pY, const float pW, const float pH)
{
	glBindTexture(GL_TEXTURE_2D, pImage->textureId());
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2i(0, 0); glVertex2d(pX,    pY  );
		glTexCoord2i(1, 0); glVertex2d(pX+pW, pY  );
		glTexCoord2i(1, 1); glVertex2d(pX+pW, pY+pH);
		glTexCoord2i(0, 1); glVertex2d(pX,    pY+pH);
	glEnd();
}

void Canvas::drawImage(const Image *pImage, const float pSX, const float pSY, const float pSW, const float pSH, const float pX, const float pY, const float pW, const float pH)
{
	float w = pImage->widthf(), h = pImage->heightf();

	float sx = pSX * w;
	float sy = pSY * h;

	float sw = pSW * w;
	float sh = pSH * h;

	glBindTexture(GL_TEXTURE_2D, pImage->textureId());
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2d(sx,	sy	 );	glVertex2d(pX,    pY   );
		glTexCoord2d(sx+sw,	sy	 );	glVertex2d(pX+pW, pY   );
		glTexCoord2d(sx+sw,	sy+sh); glVertex2d(pX+pW, pY+pH);
		glTexCoord2d(sx,	sy+sh); glVertex2d(pX,    pY+pH);
	glEnd();
}

void Canvas::drawImageBatch(const ImageBatch *pImageBatch)
{
	glBindTexture(GL_TEXTURE_2D, pImageBatch->mImage->textureId());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &pImageBatch->mVertices[0].mPos);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &pImageBatch->mVertices[0].mCoords);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &pImageBatch->mVertices[0].mColor);

	glDrawArrays(GL_QUADS, 0, pImageBatch->mVertexCount);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Canvas::setBlendMode(const BlendMode pBlendMode)
{
	switch(pBlendMode)
	{
		case NoBlend:
			glDisable(GL_BLEND);
			break;

		case AlphaBlend:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		break;

		case AddBlend:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}
		break;
	}
}

}
