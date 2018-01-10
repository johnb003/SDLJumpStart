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


FixedWidthBMPFont::FixedWidthBMPFont(const char *filename, int size)
	: size(size), m_calllistOffset(0), m_italics(false)
{
	f_texture = LoadTexture(filename);
	if (!f_texture) return;

	// The ascii bmp font included lists ascii characters: 32 to 159 (128 characters) in the top 16x8 of the texture.
	// Then it repeats the same 128 characters in italics on the bottom half.
	// We'll also create a tab character that just moves the write position. However in sum that's 257 Display Lists.
	// The characters with codes 127..159, are only a subset of the usable extended characters, so it's kind of
	// useless that we have an incomeplete set. Furthermore ascii character 127 is actually DEL, which is just
	// a blank in the font BMP.
	// So, I've decided to actually put TAB in the slot for DEL in the texture (95).
	// The reason for packing it in there, is so the indices can remain a single byte.

	// Creating 256 Display Lists
	m_calllistOffset = glGenLists(256);

	float cx;	// Holds Our X Character Coord
	float cy;	// Holds Our Y Character Coord
	for (int i = 0; i < 256; i++)
	{
		if (i == 95) continue;  // save this slot for tab (it maps to ascii 127, which is DEL anyway)
		// 16 rows, 16 cols
		cx = (i % 16) / 16.0f;            // X Position Of Current Character
		cy = (i / 16) / 16.0f;   // Y Position Of Current Character + correction

		// 0.0625f = 1/16
		glNewList(m_calllistOffset + i, GL_COMPILE);
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

	// Build the tab character; 4 spaces.
	glNewList(m_calllistOffset + 95, GL_COMPILE);
		glTranslatef(CharWidth() * 4, 0, 0);
	glEndList();
}

unsigned char FixedWidthBMPFont::AsciiToListID(char c, bool italics)
{
	// list id => ascii value
	// 0..127 => 32..159
	// 128..255 => 32..159 (italics)
	// 256 => 9 tab.
	if (c == 9) return 0;
	return italics ? (c + 96) : (c - 32);
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

	// Adds the base offset to our continuous block of callLists.
	glListBase(m_calllistOffset);

	static const int BUFF_SIZE = 32;	// number of bytes used on the stack to batch call lists for characters.
	unsigned char buff[BUFF_SIZE];
	const char *readPos = text;
	int writePos = 0;
	while (*readPos != 0)
	{
		// Instead of doing this remapping from ascii code to bmp index, it's nice to call glCallLists directly with
		// the string as the index list. This can be done by utilizing the glListBase.
		// However, then you can't validate the input or do special handling like using a special character
		// for characters not in the font, or tab alignment with tab stops, etc.
		buff[writePos++] = AsciiToListID(*readPos++, m_italics);
		if (writePos == BUFF_SIZE)
		{
			glCallLists(BUFF_SIZE, GL_UNSIGNED_BYTE, buff);
			writePos = 0;
		}
	}
	if (writePos > 0)
		glCallLists(writePos, GL_UNSIGNED_BYTE, buff);

	glPopMatrix();
	glPopAttrib();
}

int FixedWidthBMPFont::Measure(const char *text)
{
	if (text == NULL) return 0;
	return (int)(CharWidth() * strlen(text));
}

int FixedWidthBMPFont::LineHeight()
{
	return (int)CharHeight();
}
