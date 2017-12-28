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
	static const float X_SPACE_SIZE = 0.6f;
	static const float Y_SPACE_SIZE = 1.25f;

	unsigned int f_texture;
	int size;

public:
	FixedWidthBMPFont(const char *filename, int size = 16);

	virtual int Measure(const char *text);
	virtual int LineHeight();

	virtual void print(int x, int y, const char *text);

	int CharWidth() { return size * X_SPACE_SIZE; }
	int CharHeight() { return size * Y_SPACE_SIZE; }
};


#endif  // FONT_H