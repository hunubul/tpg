#pragma once

#include <map>
#include <vector>
#include "Font.h"
#include "initOpenGL.h"
#include "../definitions/Colors.h"

#include "freetype-gl.h"
#include "text-buffer.h"

std::map<TEXT_TYPE, Font> texts;
char* fontLuckiestGuyPath = "./fonts/LuckiestGuy.ttf";
char* fontZigPath = "./fonts/zig.ttf";

// Font class
void Font::CreateNewBufferGL(textBuffer& textBuff) {
	textBuff.buffer = new text_buffer_t();
	*textBuff.buffer = *generalTextBuffer;
}
void Font::Defaults() {
	fontAlign = ALIGN_LEFT;
	pen_x = pen_y = box_x = box_y = 0;
	idx = 0;
	recalculateBox = true;
	descender = 0;
	line.clear();
}
Font::Font() {
	Defaults();
	textBuffer newBuffer;
	CreateNewBufferGL(newBuffer);
	Font::fontFamily = fontZigPath;
	textBuffer::textType newMarkup = textBuffer::textType(Font::fontFamily);
	newBuffer.type.push_back(newMarkup);
	textBuffers.push_back(newBuffer);
	actualBuffer = &textBuffers[0];
}
Font::Font(char* FontPath) {
	Defaults();
	textBuffer newBuffer;
	CreateNewBufferGL(newBuffer);
	Font::fontFamily = FontPath;
	textBuffer::textType newMarkup = textBuffer::textType(Font::fontFamily);
	newBuffer.type.push_back(newMarkup);
	newMarkup.markup.foreground_color = FONTCOLOR_WHITE;
	textBuffers.push_back(newBuffer);
	actualBuffer = &textBuffers[0];
}
Font::Font(char* FontPath, const vec4& color) {
	Defaults();
	textBuffer newBuffer;
	CreateNewBufferGL(newBuffer);
	Font::fontFamily = FontPath;
	textBuffer::textType newMarkup = textBuffer::textType(Font::fontFamily);
	newBuffer.type.push_back(newMarkup);
	newMarkup.markup.foreground_color = color;
	textBuffers.push_back(newBuffer);
	actualBuffer = &textBuffers[0];
}
Font::Font(char* FontPath, float r, float g, float b) {
	Defaults();
	textBuffer newBuffer;
	CreateNewBufferGL(newBuffer);
	Font::fontFamily = FontPath;
	textBuffer::textType newMarkup = textBuffer::textType(Font::fontFamily);
	newBuffer.type.push_back(newMarkup);
	newMarkup.markup.foreground_color = { r / 255, g / 255, b / 255, 1.0 };
	textBuffers.push_back(newBuffer);
	actualBuffer = &textBuffers[0];
}
Font::Font(char* FontPath, float r, float g, float b, float a) {
	Defaults();
	textBuffer newBuffer;
	CreateNewBufferGL(newBuffer);
	Font::fontFamily = FontPath;
	textBuffer::textType newMarkup = textBuffer::textType(Font::fontFamily);
	newBuffer.type.push_back(newMarkup);
	newMarkup.markup.foreground_color = { r / 255, g / 255, b / 255, a / 255 };
	textBuffers.push_back(newBuffer);
	actualBuffer = &textBuffers[0];
}

void Font::changeFontColor(const vec4& color) {
	textBuffer::textType newMarkup("", actualBuffer->type[idx].markup);
	newMarkup.markup.foreground_color = color;
	actualBuffer->type.push_back(newMarkup);
	idx++;
}
void Font::changeFontColor(float r, float g, float b) {
	textBuffer::textType newMarkup("", actualBuffer->type[idx].markup);
	newMarkup.markup.foreground_color = { r / 255, g / 255, b / 255, 1.0 };
	actualBuffer->type.push_back(newMarkup);
	idx++;
}
void Font::changeFontColor(float r, float g, float b, float a) {
	textBuffer::textType newMarkup("", actualBuffer->type[idx].markup);
	newMarkup.markup.foreground_color = { r / 255, g / 255, b / 255, a / 255 };
	actualBuffer->type.push_back(newMarkup);
	idx++;
}
void Font::changeBackgroundColor(const vec4& color) {
	textBuffer::textType newMarkup("", actualBuffer->type[idx].markup);
	newMarkup.markup.background_color = color;
	actualBuffer->type.push_back(newMarkup);
	idx++;
}
void Font::changeBackgroundColor(float r, float g, float b) {
	textBuffer::textType newMarkup("", actualBuffer->type[idx].markup);
	newMarkup.markup.background_color = { r / 255, g / 255, b / 255, 1.0 };
	actualBuffer->type.push_back(newMarkup);
	idx++;
}
void Font::changeBackgroundColor(float r, float g, float b, float a) {
	textBuffer::textType newMarkup("", actualBuffer->type[idx].markup);
	newMarkup.markup.background_color = { r / 255, g / 255, b / 255, a / 255 };
	actualBuffer->type.push_back(newMarkup);
	idx++;
}

void Font::setFontColor(const vec4& color) {
	actualBuffer->type[idx].markup.foreground_color = color;
}
void Font::setBackgroundColor(const vec4& color) {
	actualBuffer->type[idx].markup.background_color = color;
}
void Font::setFontType(char* FontPath) {
	actualBuffer->type[idx].markup.family = FontPath;
}
void Font::setFontSize(float fontSize) {
	actualBuffer->type[idx].markup.size = fontSize;
}
void Font::setDescender(float desc) {
	descender = desc;
}

void Font::changeBuffer(size_t nidx) {
	if (textBuffers.size() == nidx) {
		Defaults();
		textBuffer newBuffer;
		CreateNewBufferGL(newBuffer);
		textBuffer::textType newMarkup = textBuffer::textType(Font::fontFamily);
		newBuffer.type.push_back(newMarkup);
		textBuffers.push_back(newBuffer);
		actualBuffer = &textBuffers[textBuffers.size()-1];
	} else if (nidx < textBuffers.size()) {
		actualBuffer = &textBuffers[nidx];
	}
}

void Font::append(const std::string& str) {
	actualBuffer->type[idx].text.append(str);
	line.append(str);
}
void Font::Clear() {
	Defaults();

	Font::fontFamily = fontZigPath;
	textBuffer::textType newMarkup = textBuffer::textType(Font::fontFamily);
	for (int i = 0;i<textBuffers.size();i++) {
		textBuffers[i].type.clear();
		textBuffers[i].type.push_back(newMarkup);
	}

	actualBuffer = &textBuffers[0];
}
void Font::Clear(char* FontPath, float fontSize, const vec4& fontColor) {
	Defaults();

	Font::fontFamily = FontPath;
	textBuffer::textType newMarkup = textBuffer::textType(Font::fontFamily);
	newMarkup.markup.size = fontSize;
	newMarkup.markup.foreground_color = fontColor;
	for (int i = 0; i<textBuffers.size(); i++) {
		textBuffers[i].type.clear();
		textBuffers[i].type.push_back(newMarkup);
	}

	actualBuffer = &textBuffers[0];
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

textBuffer::textType::textType(char* fontFamily) {
	//text.clear();

	markup.family = fontFamily;
	markup.size = 128.0;
	markup.bold = 0;
	markup.italic = 0;
	markup.rise = 0.0;
	markup.spacing = 0.0;
	markup.gamma = 1.0;
	markup.foreground_color = FONTCOLOR_BLACK;
	markup.background_color = FONTCOLOR_NONE;
	markup.outline = 0;
	markup.outline_color = FONTCOLOR_BLACK;
	markup.underline = 0;
	markup.underline_color = FONTCOLOR_BLACK;
	markup.overline = 0;
	markup.overline_color = FONTCOLOR_BLACK;
	markup.strikethrough = 0;
	markup.strikethrough_color = FONTCOLOR_BLACK;
	markup.font = nullptr; // font_manager_get_from_markup(FontBuffer->manager, &FontMarkup);
	//text_buffer_add_text(FontBuffer, &pen, &FontMarkup, "1234567890", 10);
}

textBuffer::textType::textType(const std::string& text_in, const markup_t& FontMarkup_in) {
	text = text_in;
	markup = FontMarkup_in;
}

void Font::ProcessLine() {
	if(!actualBuffer->type[idx].markup.font)
		actualBuffer->type[idx].markup.font = font_manager_get_from_markup(actualBuffer->buffer->manager, &actualBuffer->type[idx].markup);

	if (recalculateBox) {
		float tempWidth = 0, tempHeight = 0;
		if (actualBuffer->type[idx].markup.font != nullptr) {

			for (size_t i = 0; i < line.length(); i++) {
				const char codepoint[2] = { line[i] , '\0' };
				texture_glyph_t *charGlyph = texture_font_get_glyph(actualBuffer->type[idx].markup.font, codepoint);
				if (charGlyph != nullptr)
					tempWidth += charGlyph->advance_x;
			}
			
			actualBuffer->type[idx].markup.font->descender = descender;
			box_y += actualBuffer->type[idx].markup.font->ascender + descender;
		}

		if (box_x < tempWidth)
			box_x = tempWidth;
	}
}