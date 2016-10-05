#pragma once

#include <map>
#include <vector>

#include "freetype-gl.h"
#include "text-buffer.h"

class Font;

// Text Vector for writing to screen
enum TEXT_TYPE { MINIMAP_TEXT = 0, ARROW_TEXT, _SIZE };
extern std::map<TEXT_TYPE, Font> texts;

class Font {
private:
	int idx;
	std::string line;
	float descender;

	void Defaults();
	void ProcessLine();
public:
	class textType {
	public:
		std::string text;
		markup_t FontMarkup;
		textType(char* fontFamily);
		textType(const std::string& text_in, const markup_t& FontMarkup);
	};
	std::vector<textType> text;
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
	void setFontSize(float fontSize);
	void append(const std::string& str);
	void changeDescender(float desc);

	void NewLine();
	void Finish(); //To calculate box
};