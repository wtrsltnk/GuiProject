/*
 * Font.h
 *
 *  Created on: Mar 12, 2011
 *      Author: wouter
 */

#ifndef FONT_H_
#define FONT_H_

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include <GL/gl.h>

namespace ui
{

// This is from imgui, recast navigation code
inline unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255)
{
	return (r) | (g << 8) | (b << 16) | (a << 24);
}

class Font
{
public:
	Font();
	virtual ~Font();

	bool initializeFont(const char* fontpath);
	int getTextLength(const char* text, int count = 0);
	int getTextHeight(const char* text);
	void drawText(float x, float y, const char *text, unsigned int col);
	void getBakedQuad(int pw, int ph, int char_index, float *xpos, float *ypos, stbtt_aligned_quad *q);

	stbtt_bakedchar mCharData[96]; // ASCII 32..126 is 95 glyphs
	GLuint mTextureId;

};

}

#endif /* FONT_H_ */
