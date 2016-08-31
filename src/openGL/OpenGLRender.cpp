// OpenGL headers
#define GLEW_STATIC
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
#include "Shader.h"
#include "Camera.h"
#include "Vertices.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>
//#define FONS_USE_FREETYPE
#include "fontstash.h"
//#include "glfontstash.h"
#include "gl3fontstash.h"

SDL_Event sdlEvent;

// Function prototypes
void Do_Movement();
void ManageEvents();
void render3Dmodells();
void RenderSubsection(int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight);
void setDirtyParams();
void DrawText();

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

// The MAIN function, from here we start our application and run our Game loop
void openGLloop()
{
	//unsigned char* texture1Arr = new unsigned char[screenWidth *screenHeight * 3];

	// Game loop
	while (!quit) {
		ManageEvents();

		// Set frame time
		GLfloat currentFrame = (GLfloat)SDL_GetTicks() / 1000;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		Do_Movement();

		// Place and transform 3D to screen
		render3Dmodells();

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

		/*GLenum err = GL_NO_ERROR;
		while ((err = glGetError()) != GL_NO_ERROR) {
			printf("Error: %x\n", err);
		}*/

		// Render subsections
		setDirtyParams();
		RenderSubsection(dirtyX[0], dirtyY[0], dirtyWidth[0], dirtyHeight[0]);
		RenderSubsection(dirtyX[1], dirtyY[1], dirtyWidth[1], dirtyHeight[1]);

		glBindVertexArray(VAO_FrameBuff);

		framebufferShader->UseProgram();
		glBindImageTexture(0, texColorBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(1, ASCIITexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(2, ASCIIMaxIndexes, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16F);
		glBindImageTexture(3, prevframeTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
		glUniform1i(0, (GLint)ASCIION);
		glUniform1iv(1, MAX_NUM_OF_DIRTY_BLOCKS, dirtyX.data());
		glUniform1iv(1+   MAX_NUM_OF_DIRTY_BLOCKS, MAX_NUM_OF_DIRTY_BLOCKS, dirtyY.data());
		glUniform1iv(1+2* MAX_NUM_OF_DIRTY_BLOCKS, MAX_NUM_OF_DIRTY_BLOCKS, dirtyWidth.data());
		glUniform1iv(1+3* MAX_NUM_OF_DIRTY_BLOCKS, MAX_NUM_OF_DIRTY_BLOCKS, dirtyHeight.data());

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// Update and render Font text
		DrawText();

		// Swap the buffers
		SDL_GL_SwapWindow(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteVertexArrays(1, &VAO_FrameBuff);
	glDeleteBuffers(1, &VBO_FrameBuff);
	glDeleteVertexArrays(1, &VAOfront);
	glDeleteBuffers(1, &VBOfront);
	glDeleteVertexArrays(1, &VAOleft);
	glDeleteBuffers(1, &VBOleft);
	glDeleteVertexArrays(1, &VAOright);
	glDeleteBuffers(1, &VBOright);
	glDeleteVertexArrays(1, &VAOfloor);
	glDeleteBuffers(1, &VBOfloor);
	glDeleteVertexArrays(1, &VAOceiling);
	glDeleteBuffers(1, &VBOceiling);
	gl3fonsDelete(fs);

	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void ManageEvents() {
	while (SDL_PollEvent(&sdlEvent) != 0) {
		// Esc button is pressed
		switch (sdlEvent.type) {
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
				quit = true;
			if (sdlEvent.key.keysym.sym == SDLK_f) {
				if (ASCIION) {
					ASCIION = false;
				} else {
					ASCIION = true;
					firstRender = true;
				}
			}
			if (sdlEvent.key.keysym.sym >= 0 && sdlEvent.key.keysym.sym < 1024)
				keys[sdlEvent.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			if (sdlEvent.key.keysym.sym >= 0 && sdlEvent.key.keysym.sym < 1024)
				keys[sdlEvent.key.keysym.sym] = false;
			break;
		case SDL_MOUSEMOTION:
			camera.ProcessMouseMovement((GLfloat)sdlEvent.motion.xrel, -(GLfloat)sdlEvent.motion.yrel);
			break;
		case SDL_MOUSEWHEEL:
			camera.ProcessMouseScroll((GLfloat)sdlEvent.wheel.y);
			break;
		case SDL_QUIT:
			quit = true;
			break;
		default:
			break;
		}
	}
}

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

void render3Dmodells() {
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
		newRoom = false;
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

void DrawText() {
	float sx, sy, dx, dy, lh = 0;
	unsigned int white, black, brown, blue;
	// Update and render
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenWidth, screenHeight, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glColor4ub(255, 255, 255, 255);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);


	GLfloat mat[16];

	memset(mat, 0, 16 * sizeof(GLfloat));
	mat[0] = (GLfloat)2.0 / screenWidth;
	mat[5] = (GLfloat)-2.0 / screenHeight;
	mat[10] = 2.0;
	mat[12] = -1.0;
	mat[13] = 1.0;
	mat[14] = -1.0;
	mat[15] = 1.0;

	gl3fonsProjection(fs, mat);

	white = gl3fonsRGBA(255, 255, 255, 255);
	brown = gl3fonsRGBA(192, 128, 0, 128);
	blue = gl3fonsRGBA(0, 192, 255, 255);
	black = gl3fonsRGBA(0, 0, 0, 255);

	sx = 50; sy = 50;

	dx = sx; dy = sy;

	//DrawBox(0,0,screenWidth,200);
	DrawDash(dx, dy);

	fonsClearState(fs);

	fonsSetSize(fs, 124.0f);
	fonsSetFont(fs, fontNormal);
	fonsVertMetrics(fs, NULL, NULL, &lh);
	dx = sx;
	dy += lh;
	DrawDash(dx, dy);

	fonsSetSize(fs, 124.0f);
	fonsSetFont(fs, fontNormal);
	fonsSetColor(fs, white);
	int dTime = (int)(1.0 / deltaTime);
	std::string dTimeStr = std::to_string(dTime);
	dx = fonsDrawText(fs, dx, dy, dTimeStr.c_str(), NULL);

	fonsSetSize(fs, 48.0f);
	fonsSetFont(fs, fontItalic);
	fonsSetColor(fs, brown);
	dx = fonsDrawText(fs, dx, dy, "brown ", NULL);

	fonsSetSize(fs, 24.0f);
	fonsSetFont(fs, fontNormal);
	fonsSetColor(fs, white);
	float dPos = camera.Position.z;
	std::string dPosStr = std::to_string(dPos);
	dx = fonsDrawText(fs, dx, dy, dPosStr.c_str(), NULL);

	fonsVertMetrics(fs, NULL, NULL, &lh);
	dx = sx;
	dy += lh*1.2f;
	DrawDash(dx, dy);
	fonsSetFont(fs, fontItalic);
	dx = fonsDrawText(fs, dx, dy, "jumps over ", NULL);
	fonsSetFont(fs, fontBold);
	dx = fonsDrawText(fs, dx, dy, "the lazy ", NULL);
	fonsSetFont(fs, fontNormal);
	dx = fonsDrawText(fs, dx, dy, "dog.", NULL);

	dx = sx;
	dy += lh*1.2f;
	DrawDash(dx, dy);
	fonsSetSize(fs, 12.0f);
	fonsSetFont(fs, fontNormal);
	fonsSetColor(fs, blue);
	fonsDrawText(fs, dx, dy, "Now is the time for all good men to come to the aid of the party.", NULL);

	//For Unicode characters
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert; // #include <codecvt>
	std::string utf8_string;

	fonsVertMetrics(fs, NULL, NULL, &lh);
	dx = sx;
	dy += lh*1.2f * 2;
	DrawDash(dx, dy);
	fonsSetSize(fs, 18.0f);
	fonsSetFont(fs, fontItalic);
	fonsSetColor(fs, white);
	utf8_string = convert.to_bytes(L"Ég get etið gler án þess að meiða mig.");
	fonsDrawText(fs, dx, dy, utf8_string.c_str(), NULL);

	fonsVertMetrics(fs, NULL, NULL, &lh);
	dx = sx;
	dy += lh*1.2f;
	DrawDash(dx, dy);
	fonsSetFont(fs, fontJapanese);
	utf8_string = convert.to_bytes(L"私はガラスを食べられます。それは私を傷つけません。");
	fonsDrawText(fs, dx, dy, utf8_string.c_str(), NULL);

	// Font alignment
	fonsSetSize(fs, 18.0f);
	fonsSetFont(fs, fontNormal);
	fonsSetColor(fs, white);

	dx = 50; dy = 350;
	DrawLine(dx - 10, dy, dx + 250, dy);
	fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
	dx = fonsDrawText(fs, dx, dy, "Top", NULL);
	dx += 10;
	fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_MIDDLE);
	dx = fonsDrawText(fs, dx, dy, "Middle", NULL);
	dx += 10;
	fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE);
	dx = fonsDrawText(fs, dx, dy, "Baseline", NULL);
	dx += 10;
	fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_BOTTOM);
	fonsDrawText(fs, dx, dy, "Bottom", NULL);

	dx = 150; dy = 400;
	DrawLine(dx, dy - 30, dx, dy + 80.0f);
	fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE);
	fonsDrawText(fs, dx, dy, "Left", NULL);
	dy += 30;
	fonsSetAlign(fs, FONS_ALIGN_CENTER | FONS_ALIGN_BASELINE);
	fonsDrawText(fs, dx, dy, "Center", NULL);
	dy += 30;
	fonsSetAlign(fs, FONS_ALIGN_RIGHT | FONS_ALIGN_BASELINE);
	fonsDrawText(fs, dx, dy, "Right", NULL);

	// Blur
	dx = 500; dy = 350;
	fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE);

	fonsSetSize(fs, 60.0f);
	fonsSetFont(fs, fontItalic);
	fonsSetColor(fs, white);
	fonsSetSpacing(fs, 5.0f);
	fonsSetBlur(fs, 10.0f);
	fonsDrawText(fs, dx, dy, "Blurry...", NULL);

	dy += 50.0f;

	fonsSetSize(fs, 18.0f);
	fonsSetFont(fs, fontBold);
	fonsSetColor(fs, black);
	fonsSetSpacing(fs, 0.0f);
	fonsSetBlur(fs, 3.0f);
	fonsDrawText(fs, dx, dy + 2, "DROP THAT SHADOW", NULL);

	fonsSetColor(fs, white);
	fonsSetBlur(fs, 0);
	fonsDrawText(fs, dx, dy, "DROP THAT SHADOW", NULL);

	/*if (debug)
	fonsDrawDebug(fs, 800.0, 50.0);*/

	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}