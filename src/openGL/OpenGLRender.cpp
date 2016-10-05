// OpenGL headers
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

// SDL headers
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>

// Std. Includes
#include <string>
#include <cmath>
#include <iostream>
#include <codecvt>
#include <algorithm>

// GL includes
#include "ASCII.h"
#include "initOpenGL.h"
#include "OpenGLRender.h"
#include "Shader.h"
#include "Camera.h"
#include "Vertices.h"
#include "../globals.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>
#include "freetype-gl.h"
#include "text-buffer.h"
#include "../definitions/Colors.h"

SDL_Event sdlEvent;
std::map<TEXT_TYPE, Font> texts;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

bool ASCIION = true;
bool quit = false;
bool CPUaccel = false;
bool firstRender = true;
bool newRoom = true;

std::vector<int> dirtyX(MAX_NUM_OF_DIRTY_BLOCKS, -1);
std::vector<int> dirtyY(MAX_NUM_OF_DIRTY_BLOCKS, -1);
std::vector<int> dirtyWidth(MAX_NUM_OF_DIRTY_BLOCKS, -1);
std::vector<int> dirtyHeight(MAX_NUM_OF_DIRTY_BLOCKS, -1);

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[SDLK_w])
		camera.ProcessKeyboard(CAM::FORWARD, deltaTime);
	if (keys[SDLK_s])
		camera.ProcessKeyboard(CAM::BACKWARD, deltaTime);
	if (keys[SDLK_a])
		camera.ProcessKeyboard(CAM::LEFT, deltaTime);
	if (keys[SDLK_d])
		camera.ProcessKeyboard(CAM::RIGHT, deltaTime);
}

void frameLimiter() {
	GLfloat currentFrame = (GLfloat)SDL_GetTicks() / 1000;
	deltaTime = currentFrame - lastFrame;
	if (globals::limitFPSmanually && deltaTime != 0 && (int)(1.0 / deltaTime) > globals::FPS_manual_limit) {
		SDL_Delay((int)(1000.0 / globals::FPS_manual_limit - 1000.0*deltaTime));
		currentFrame = (GLfloat)SDL_GetTicks() / 1000;
		deltaTime = currentFrame - lastFrame;
	}
	lastFrame = currentFrame;
}

void render3Dmodels() {
	// Clear the colorbuffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw our first triangle
	defaultShader->UseProgram();

	//Change textures
	if (newRoom) {
		glBindTexture(GL_TEXTURE_2D, texture1);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1Arr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(defaultShader->Program, "ourTexture1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(defaultShader->Program, "ourTexture2"), 1);

	// Create camera transformation
	glm::mat4 view;
	view = camera.GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(defaultShader->Program, "model");
	GLint viewLoc = glGetUniformLocation(defaultShader->Program, "view");
	GLint projLoc = glGetUniformLocation(defaultShader->Program, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model;
	model = glm::translate(model, cubePositions[0]);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAOfront);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindVertexArray(VAOleft);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindVertexArray(VAOright);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindVertexArray(VAOfloor);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindVertexArray(VAOceiling);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	if (ASCIION) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);	 // READ: Supersampled
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameDownBuffer); // WRITE

		// Downsample the supersampled FBO using LINEAR interpolation
		glBlitFramebuffer(
			0, 0, screenWidth, screenHeight,
			0, 0, screenWidth / FontX, screenHeight / FontY,
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR
			);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setDirtyParams() {
	dirtyX[0] = 0 / FontX;
	dirtyY[0] = 0 / FontY;
	if (firstRender) {
		dirtyWidth[0] = screenWidth / FontX;
		dirtyHeight[0] = screenHeight / FontY;
		firstRender = false;
	} else {
		dirtyWidth[0] = 20;
		dirtyHeight[0] = 20;
	}
	dirtyX[1] = 30;
	dirtyY[1] = 0;
	dirtyWidth[1] = 10;
	dirtyHeight[1] = 10;
}

void RenderSubsection(int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight) {
	if (ASCIION) {
		computeASCIIShader->UseComputerShaderProgram();
		glBindImageTexture(0, texColorBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(1, texColorDownBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(2, ASCIITexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(3, ASCIIBrightness, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(4, ASCIIScreenWeights, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
		glUniform1i(0, (GLint)dirtyX);
		glUniform1i(1, (GLint)dirtyY);

		glDispatchCompute(1, dirtyWidth, dirtyHeight);

		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		if (dirtyWidth*dirtyHeight <= 27 * 27 || !CPUaccel) {
			//Faster to render using compute shader
			computeASCIIMaxIndexShader->UseComputerShaderProgram();
			glBindImageTexture(0, ASCIIScreenWeights, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
			glBindImageTexture(1, ASCIIMaxIndexes, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R16F);
			glUniform1i(0, (GLint)dirtyX);
			glUniform1i(1, (GLint)dirtyY);

			glDispatchCompute(dirtyWidth, dirtyHeight, 1);

			glMemoryBarrier(GL_ALL_BARRIER_BITS);
		} else {
			glBindTexture(GL_TEXTURE_2D, ASCIIScreenWeights);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, asciiScreenWeights);
			glBindTexture(GL_TEXTURE_2D, 0);

			float maxWeight = 0.0;
			int index = 0;

			for (int y = dirtyY; y < dirtyY + dirtyHeight; y++) {
				for (int x = dirtyX; x < dirtyX + dirtyWidth; x++) {
					for (int i = 0; i < fontSize; i++) {
						if (i == 0) {
							index = 0;
							maxWeight = asciiScreenWeights[y*(fontSize * screenWidth / FontX) + x * fontSize + i];
						}
						if (maxWeight < asciiScreenWeights[y*(fontSize * screenWidth / FontX) + x * fontSize + i]) {
							maxWeight = asciiScreenWeights[y*(fontSize * screenWidth / FontX) + x * fontSize + i];
							index = i;
						}
					}
					asciiMaxIndexes[y*screenWidth / FontX + x] = (float)index;
				}
			}

			glBindTexture(GL_TEXTURE_2D, ASCIIMaxIndexes);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, screenWidth / FontX, screenHeight / FontY, 0, GL_RED, GL_FLOAT, asciiMaxIndexes);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void drawBufferShader() {
	glBindVertexArray(VAO_FrameBuff);

	framebufferShader->UseProgram();
	glBindImageTexture(0, texColorBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	glBindImageTexture(1, ASCIITexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	glBindImageTexture(2, ASCIIMaxIndexes, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16F);
	glBindImageTexture(3, prevframeTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
	glUniform1i(0, (GLint)ASCIION);
	glUniform1iv(1, MAX_NUM_OF_DIRTY_BLOCKS, dirtyX.data());
	glUniform1iv(1 + MAX_NUM_OF_DIRTY_BLOCKS, MAX_NUM_OF_DIRTY_BLOCKS, dirtyY.data());
	glUniform1iv(1 + 2 * MAX_NUM_OF_DIRTY_BLOCKS, MAX_NUM_OF_DIRTY_BLOCKS, dirtyWidth.data());
	glUniform1iv(1 + 3 * MAX_NUM_OF_DIRTY_BLOCKS, MAX_NUM_OF_DIRTY_BLOCKS, dirtyHeight.data());

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void DrawTextGL() {
	//DrawBox(0,0,screenWidth,200);

	text_buffer_clear(FontBuffer);

	Font::textType fontFPS(fontZigPath);
	
	vec2 pen = { {0,0} };
	fontFPS.FontMarkup.size = 36.0f;
	int dTime = globals::FPS_manual_limit;
	if (deltaTime != 0) dTime = (int)(1.0 / deltaTime);
	std::string dTimeStr = std::to_string(dTime);
	fontFPS.FontMarkup.foreground_color = FONTCOLOR_BLACK;
	pen.x = (float)screenWidth - 30* dTimeStr.size();
	pen.y = (float)screenHeight - 3;
	text_buffer_printf(FontBuffer, &pen, &fontFPS.FontMarkup, dTimeStr.c_str(), NULL);

	fontFPS.FontMarkup.foreground_color = FONTCOLOR_GREEN;
	pen.x = (float)screenWidth - 30 * dTimeStr.size() - 3;
	pen.y = (float)screenHeight;
	text_buffer_printf(FontBuffer, &pen, &fontFPS.FontMarkup, dTimeStr.c_str(), NULL);

	static bool asdasd = true;
	if (asdasd) {
		for (enum TEXT_TYPE i = MINIMAP_TEXT; i < (int)texts.size(); i = (TEXT_TYPE)(i+1)) {
			pen.x = texts[i].pen_x;
			pen.y = texts[i].pen_y;
			FontBuffer->origin = { pen.x ,pen.y};
			for (size_t j = 0; j < texts[i].text.size(); j++) {
				text_buffer_printf(FontBuffer, &pen, &texts[i].text[j].FontMarkup, texts[i].text[j].text.c_str(), NULL);
			}
			//text_buffer_align(FontBuffer, &pen, texts[i].fontAlign);
		}
		//asdasd = false;
	}

	
	glBindTexture(GL_TEXTURE_2D, FontBuffer->manager->atlas->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, FontBuffer->manager->atlas->width,
		FontBuffer->manager->atlas->height, 0, GL_RED, GL_UNSIGNED_BYTE,
		FontBuffer->manager->atlas->data);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(FontBuffer->shader);
	{
		glUniformMatrix4fv(glGetUniformLocation(FontBuffer->shader, "model"),
			1, 0, FontModelMat.data);
		glUniformMatrix4fv(glGetUniformLocation(FontBuffer->shader, "view"),
			1, 0, FontViewMat.data);
		glUniformMatrix4fv(glGetUniformLocation(FontBuffer->shader, "projection"),
			1, 0, FontProjectionMat.data);
		text_buffer_render(FontBuffer);
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawUIGL() {
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	uiShader->UseProgram();

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, UIElements::Compass::textureID);
	glUniform1i(glGetUniformLocation(uiShader->Program, "ourTexture1"), 0);

	glBindVertexArray(UIElements::Compass::VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Font class
Font::Font() {
	fontAlign = ALIGN_LEFT;
	pen_x = pen_y = 0;
	fontFamily = NULL;
	idx = 0;
}
Font::Font(char* FontPath) {
	fontAlign = ALIGN_LEFT;
	pen_x = pen_y = 0;
	Font::fontFamily = FontPath;
	textType newMarkup = textType(Font::fontFamily);
	newMarkup.FontMarkup.foreground_color = FONTCOLOR_WHITE;
	text.push_back(newMarkup);
	idx = 0;
}
Font::Font(char* FontPath, const vec4& color) {
	fontAlign = ALIGN_LEFT;
	pen_x = pen_y = 0;
	Font::fontFamily = FontPath;
	textType newMarkup = textType(Font::fontFamily);
	newMarkup.FontMarkup.foreground_color = color;
	text.push_back(newMarkup);
	idx = 0;
}
Font::Font(char* FontPath, float r, float g, float b) {
	fontAlign = ALIGN_LEFT;
	pen_x = pen_y = 0;
	Font::fontFamily = FontPath;
	textType newMarkup = textType(Font::fontFamily);
	newMarkup.FontMarkup.foreground_color = { r / 255, g / 255, b / 255, 1.0 };
	text.push_back(newMarkup);
	idx = 0;
}
Font::Font(char* FontPath, float r, float g, float b, float a) {
	fontAlign = ALIGN_LEFT;
	pen_x = pen_y = 0;
	Font::fontFamily = FontPath;
	textType newMarkup = textType(Font::fontFamily);
	newMarkup.FontMarkup.foreground_color = { r / 255, g / 255, b / 255, a / 255 };
	text.push_back(newMarkup);
	idx = 0;
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
	textType newMarkup("",text[idx].FontMarkup);
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
	//text[idx].FontMarkup.font = font_manager_get_from_markup(FontBuffer->manager, &text[idx].FontMarkup);
}
void Font::append(const std::string& str) {
	text[idx].text.append(str);
}
void Font::NewLine() {
	Font::append("\n");
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
	FontMarkup.font = 0;// font_manager_get_from_markup(FontBuffer->manager, &FontMarkup);
	//text_buffer_add_text(FontBuffer, &pen, &FontMarkup, "1234567890", 10);
}

Font::textType::textType(const std::string& text_in, const markup_t& FontMarkup_in) {
	text = text_in;
	FontMarkup = FontMarkup_in;
}