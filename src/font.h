#ifndef FONT_H
#define FONT_H

// Fonts should contain the ability to rasterize characters on screen.
// Thus for each character they should know width and height.
// And may contain kerning pairs too.
class IFont
{
public:
	virtual ~IFont() {}

	/// Returns the width of a string in pixels.
	/// If the string contains newlines, the width is considered the longest line.
	virtual int Measure(const char *text) = 0;

	/// Returns the number of pixels to move vertically for a new line of text.
	/// TODO: CHANGE: don't imply this low level line spacing. Leave that up to the usage. This should measure
	///       the height of the font in pixels, or handle multi-line string height?
	virtual int LineHeight() = 0;

	// TODO: Have a text layout class, with options for wrap n'stuff, and simplify what this does.
	virtual void print(int x, int y, const char *text) = 0;
};


// This is a super old, legacy bmp font from ages ago.
// This will be replaced with a more appropriate font system.
// This is the reason for IFont, so that there's a reasonable interface
// now, that should be similar after this goes away.
class FixedWidthBMPFont : public IFont
{
	static const float GLYPH_PERCENT_OF_CELL_X;
	static const float GLYPH_PERCENT_OF_CELL_Y;
	static const float GLPYH_CELL_PADDING_X;
	static const float GLPYH_CELL_PADDING_Y;
	static const float GLPYH_CENTER_OFFSET_X;
	static const float GLPYH_CENTER_OFFSET_Y;

	unsigned int f_texture;
	int size;

public:
	FixedWidthBMPFont(const char *filename, int size = 16);

	virtual int Measure(const char *text);
	virtual int LineHeight();

	virtual void print(int x, int y, const char *text);

	// These return floats because they otherwise imply too much rounding.
	// Use round, ceil, or truncate as you see fit.
	float CharWidth() { return size * GLYPH_PERCENT_OF_CELL_X; }
	float CharHeight() { return size * GLYPH_PERCENT_OF_CELL_Y; }
};


#endif  // FONT_H