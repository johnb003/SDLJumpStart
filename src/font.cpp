#include "font.h"
#include "sdl_gl_texture.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif  // _WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#endif  // Anything other than __APPLE__

#include <string>
#include <sstream>

//  The * aread makes up GLYPH_PERCENT_OF_CELL_X (% of the square)
//  The margin on left and right is (1-GLYPH_PERCENT_OF_CELL_X)/2.0f
//  |  ******  |
//  |  ******  |
//  |  ******  |
//  |  ******  |

const float FixedWidthBMPFont::GLYPH_PERCENT_OF_CELL_X = 0.6f;  // 60% of full box width
const float FixedWidthBMPFont::GLYPH_PERCENT_OF_CELL_Y = 1.0f;  // Was 1.25; Don't add extra padding, let the app do that.
const float FixedWidthBMPFont::GLPYH_CELL_PADDING_X = (1-FixedWidthBMPFont::GLYPH_PERCENT_OF_CELL_X)/2.0f;
const float FixedWidthBMPFont::GLPYH_CELL_PADDING_Y = (1-FixedWidthBMPFont::GLYPH_PERCENT_OF_CELL_Y)/2.0f;
const float FixedWidthBMPFont::GLPYH_CENTER_OFFSET_X = 0.1f;
const float FixedWidthBMPFont::GLPYH_CENTER_OFFSET_Y = 0.0f;


FixedWidthBMPFont::FixedWidthBMPFont(const char *filename, int size) : size(size)
{
	f_texture = LoadTexture(filename);
	if (!f_texture) return;

	// Creating 256 Display Lists
	unsigned int base = glGenLists(257);

	// Build the tab character; 4 spaces.
	glNewList(9, GL_COMPILE);
		glTranslatef(CharWidth() * 4, 0, 0);
	glEndList();

	float cx;	// Holds Our X Character Coord
	float cy;	// Holds Our Y Character Coord
	for (int loop = 0; loop < 256; loop++)
	{
		// 16 rows, 16 cols
		cx = (loop % 16) / 16.0f;            // X Position Of Current Character
		cy = (loop / 16) / 16.0f;   // Y Position Of Current Character + correction

		// 0.0625f = 1/16
		glNewList(32 + loop, GL_COMPILE);
			// Slightly offset from print location to align text.
			float divy = 0.0625f;
//			float yOff = -0.1f * divy;
			glBegin(GL_QUADS);
				glTexCoord2f(cx,        cy);        glVertex2d(0,    0);
				glTexCoord2f(cx,        cy + divy); glVertex2d(0,    size);
				glTexCoord2f(cx + divy, cy + divy); glVertex2d(size, size);
				glTexCoord2f(cx + divy, cy);        glVertex2d(size, 0);
			glEnd();

			// Advance the matrix for the next character.
			glTranslatef(CharWidth(), 0, 0);
		glEndList();
	}
}

void FixedWidthBMPFont::print(int x, int y, const char *text)
{
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, f_texture);

	glColor4f(1, 1, 1, 1);
	glPushMatrix();
	glTranslated(x, y, 0);
	
	glTranslated(-(GLPYH_CELL_PADDING_X + GLPYH_CENTER_OFFSET_X) * size,
				 -(GLPYH_CELL_PADDING_Y + GLPYH_CENTER_OFFSET_Y) * size, 0);
	glCallLists(strlen(text), GL_BYTE, text);
	glPopMatrix();
	glPopAttrib();
};

int FixedWidthBMPFont::Measure(const char *text)
{
	if (text == NULL) return 0;
	return (int)(CharWidth() * strlen(text));
}

int FixedWidthBMPFont::LineHeight()
{
	return (int)CharHeight();
}
