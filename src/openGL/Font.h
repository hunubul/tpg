#pragma once

#include <map>
#include <vector>

#include "freetype-gl.h"
#include "text-buffer.h"

class Font;

// Text Vector for writing to screen
enum TEXT_TYPE { MINIMAP_TEXT = 0, ARROW_TEXT, _SIZE };
extern std::map<TEXT_TYPE, Font> texts;

extern char* fontLuckiestGuyPath;
extern char* fontZigPath;

class textBuffer {
public:
	class textType {
	public:
		std::string text;
		markup_t markup;
		textType(char* fontFamily);
		textType(const std::string& text_in, const markup_t& FontMarkup_in);
	};

	text_buffer_t* buffer;
	std::vector<textType> type;
};

class Font {
private:
	textBuffer* actualBuffer;
	size_t idx;
	std::string line;
	float descender;

	void CreateNewBufferGL(textBuffer& textBuff);
	void Defaults();
	void ProcessLine();
public:
	std::vector<textBuffer> textBuffers;
	enum Align fontAlign;
	float pen_x, pen_y;
	float box_x, box_y;
	bool recalculateBox;
	//float origin_x, origin_y;
	//bool shadow;
	char* fontFamily;

	Font();
	Font(char* FontPath);
	Font(char* FontPath, const vec4& color);
	Font(char* FontPath, float r, float g, float b);
	Font(char* FontPath, float r, float g, float b, float a);

	void changeFontColor(const vec4& color);
	void changeFontColor(float r, float g, float b);
	void changeFontColor(float r, float g, float b, float a);
	void changeBackgroundColor(const vec4& color);
	void changeBackgroundColor(float r, float g, float b);
	void changeBackgroundColor(float r, float g, float b, float a);

	void setFontColor(const vec4& color);
	void setBackgroundColor(const vec4& color);
	void setFontType(char* FontPath);
	void setFontSize(float fontSize);
	void setDescender(float desc);
	void changeBuffer(size_t nidx);

	void append(const std::string& str);
	void Clear();
	void Clear(char* FontPath, float fontSize, const vec4& fontColor);
	void NewLine();
	void Finish(); //To calculate box
};