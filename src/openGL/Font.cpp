#pragma once

#include <map>
#include <vector>
#include "Font.h"
#include "initOpenGL.h"
#include "../definitions/Colors.h"

#include "freetype-gl.h"
#include "text-buffer.h"

std::map<TEXT_TYPE, Font> texts;

// Font class
void Font::Defaults() {
	fontAlign = ALIGN_LEFT;
	pen_x = pen_y = box_x = box_y = 0;
	idx = 0;
	recalculateBox = true;
	descender = 0;
}
Font::Font() {
	Defaults();
	fontFamily = NULL;
}
Font::Font(char* FontPath) {
	Defaults();
	Font::fontFamily = FontPath;
	textType newMarkup = textType(Font::fontFamily);
	newMarkup.FontMarkup.foreground_color = FONTCOLOR_WHITE;
	text.push_back(newMarkup);
}
Font::Font(char* FontPath, const vec4& color) {
	Defaults();
	Font::fontFamily = FontPath;
	textType newMarkup = textType(Font::fontFamily);
	newMarkup.FontMarkup.foreground_color = color;
	text.push_back(newMarkup);
}
Font::Font(char* FontPath, float r, float g, float b) {
	Defaults();
	Font::fontFamily = FontPath;
	textType newMarkup = textType(Font::fontFamily);
	newMarkup.FontMarkup.foreground_color = { r / 255, g / 255, b / 255, 1.0 };
	text.push_back(newMarkup);
}
Font::Font(char* FontPath, float r, float g, float b, float a) {
	Defaults();
	Font::fontFamily = FontPath;
	textType newMarkup = textType(Font::fontFamily);
	newMarkup.FontMarkup.foreground_color = { r / 255, g / 255, b / 255, a / 255 };
	text.push_back(newMarkup);
}

void Font::changeFontColor(const vec4& color) {
	textType newMarkup("", text[idx].FontMarkup);
	newMarkup.FontMarkup.foreground_color = color;
	text.push_back(newMarkup);
	idx++;
}
void Font::changeFontColor(float r, float g, float b) {
	textType newMarkup("", text[idx].FontMarkup);
	newMarkup.FontMarkup.foreground_color = { r / 255, g / 255, b / 255, 1.0 };
	text.push_back(newMarkup);
	idx++;
}
void Font::changeFontColor(float r, float g, float b, float a) {
	textType newMarkup("", text[idx].FontMarkup);
	newMarkup.FontMarkup.foreground_color = { r / 255, g / 255, b / 255, a / 255 };
	text.push_back(newMarkup);
	idx++;
}
void Font::changeBackgroundColor(const vec4& color) {
	textType newMarkup("", text[idx].FontMarkup);
	newMarkup.FontMarkup.background_color = color;
	text.push_back(newMarkup);
	idx++;
}
void Font::changeBackgroundColor(float r, float g, float b) {
	textType newMarkup("", text[idx].FontMarkup);
	newMarkup.FontMarkup.background_color = { r / 255, g / 255, b / 255, 1.0 };
	text.push_back(newMarkup);
	idx++;
}
void Font::changeBackgroundColor(float r, float g, float b, float a) {
	textType newMarkup("", text[idx].FontMarkup);
	newMarkup.FontMarkup.background_color = { r / 255, g / 255, b / 255, a / 255 };
	text.push_back(newMarkup);
	idx++;
}
void Font::setFontSize(float fontSize) {
	text[idx].FontMarkup.size = fontSize;
}
void Font::append(const std::string& str) {
	text[idx].text.append(str);
	line.append(str);
}
void Font::changeDescender(float desc) {
	descender = desc;
}
void Font::NewLine() {
	ProcessLine();
	Font::append("\n");
	line.clear();
}
void Font::Finish() {
	ProcessLine();
	recalculateBox = false;
	line.clear();
}

Font::textType::textType(char* fontFamily) {
	text = "";

	FontMarkup.family = fontFamily;
	FontMarkup.size = 128.0;
	FontMarkup.bold = 0;
	FontMarkup.italic = 0;
	FontMarkup.rise = 0.0;
	FontMarkup.spacing = 0.0;
	FontMarkup.gamma = 1.0;
	FontMarkup.foreground_color = FONTCOLOR_BLACK;
	FontMarkup.background_color = FONTCOLOR_NONE;
	FontMarkup.outline = 0;
	FontMarkup.outline_color = FONTCOLOR_BLACK;
	FontMarkup.underline = 0;
	FontMarkup.underline_color = FONTCOLOR_BLACK;
	FontMarkup.overline = 0;
	FontMarkup.overline_color = FONTCOLOR_BLACK;
	FontMarkup.strikethrough = 0;
	FontMarkup.strikethrough_color = FONTCOLOR_BLACK;
	FontMarkup.font = nullptr; // font_manager_get_from_markup(FontBuffer->manager, &FontMarkup);
	//text_buffer_add_text(FontBuffer, &pen, &FontMarkup, "1234567890", 10);
}

Font::textType::textType(const std::string& text_in, const markup_t& FontMarkup_in) {
	text = text_in;
	FontMarkup = FontMarkup_in;
}

void Font::ProcessLine() {
	if (!text[idx].FontMarkup.font)
		text[idx].FontMarkup.font = font_manager_get_from_markup(FontBuffer->manager, &text[idx].FontMarkup);

	if (recalculateBox) {
		float tempWidth = 0, tempHeight = 0;
		if (text[idx].FontMarkup.font != nullptr) {

			for (size_t i = 0; i < line.length(); i++) {
				const char codepoint[2] = { line[i] , '\0' };
				texture_glyph_t *charGlyph = texture_font_get_glyph(text[idx].FontMarkup.font, codepoint);
				if (charGlyph != nullptr)
					tempWidth += charGlyph->advance_x;
			}
			
			text[idx].FontMarkup.font->descender = descender;
			box_y += text[idx].FontMarkup.font->ascender + descender;
		}

		if (box_x < tempWidth)
			box_x = tempWidth;
	}
}