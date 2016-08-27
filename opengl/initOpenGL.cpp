﻿// OpenGL headers
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
#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"
#define GLFONTSTASH_IMPLEMENTATION
//#include "glfontstash.h"
#include "gl3fontstash.h"

SDL_Window* window;
SDL_GLContext glContext;

// Properties
const GLuint screenWidth = 1600, screenHeight = 900;

//Shaders location
std::string texturesVertPath = "./shaders/textures.vert";
std::string texturesFragPath = "./shaders/textures.frag";
std::string framebufferVertPath = "./shaders/framebuffer.vert";
std::string framebufferFragPath = "./shaders/framebuffer.frag";
std::string computeASCIIShaderPath = "./shaders/computeASCII.shd";
std::string computeASCIIMaxIndexShaderPath = "./shaders/computeASCIIMaxIndex.shd";

// OpenGL VAO, VBO
GLuint VAO_FrameBuff, VBO_FrameBuff;
GLuint VAOfront, VBOfront;
GLuint VAOleft, VBOleft;
GLuint VAOright, VBOright;
GLuint VAOfloor, VBOfloor;
GLuint VAOceiling, VBOceiling;

GLuint texture1, texture2;
GLuint frameBuffer, texColorBuffer;
GLuint frameDownBuffer, texColorDownBuffer;
GLuint ASCIITexture, ASCIIBrightness, ASCIIScreenWeights, ASCIIMaxIndexes;

float *asciiScreenWeights, *asciiMaxIndexes;
int fontSize;

// Shaders
Shader* defaultShader;
Shader* framebufferShader;
ComputeShader* computeASCIIShader;
ComputeShader* computeASCIIMaxIndexShader;

// Function prototypes
void LoadFonts();
FONScontext* fs = NULL;
int fontNormal, fontItalic, fontBold, fontJapanese;

int gcd(int a, int b); //Greatest Common Divisor
void ClaculateVertices();
void InitSideVAOVBO(GLuint &VAO, GLuint &VBO, GLfloat(*vertices)[CubeVertices::ArrSize]);


void initOpenGL() {
	//Use OpenGL 4.3 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Initialize video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		// Display error message
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return;
	}
	// Create window
	window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return;
	}
	// Create OpenGL context
	glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL) {
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return;
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();
	GLenum err = glGetError(); //Catch error code 0x500 (1280)

							   // Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	std::vector<int> fragmentParams = { (int)screenWidth, (int)screenHeight, (int)FontX, (int)FontY, (int)fontChar.size() };
	std::vector<int> computeParams = { (int)fontChar.size(), (int)fontChar.size() / 4 };
	defaultShader = new Shader(texturesVertPath, texturesFragPath, std::vector<int>());
	framebufferShader = new Shader(framebufferVertPath, framebufferFragPath, fragmentParams);
	computeASCIIShader = new ComputeShader(computeASCIIShaderPath, computeParams);
	computeASCIIMaxIndexShader = new ComputeShader(computeASCIIMaxIndexShaderPath, std::vector<int>());

	// Fonts generation
	fs = gl3fonsCreate(512, 512, FONS_ZERO_TOPLEFT);
	if (!fs) {
		fprintf(stderr, "Could not create stash.\n");
		return;
	}
	LoadFonts();

	ClaculateVertices();

	InitSideVAOVBO(VAOfront, VBOfront, &CubeVertices::Front);
	InitSideVAOVBO(VAOleft, VBOleft, &CubeVertices::Left);
	InitSideVAOVBO(VAOright, VBOright, &CubeVertices::Right);
	InitSideVAOVBO(VAOfloor, VBOfloor, &CubeVertices::Floor);
	InitSideVAOVBO(VAOceiling, VBOceiling, &CubeVertices::Ceiling);

	//GLuint VAO_FrameBuff, VBO_FrameBuff;
	{
		glGenVertexArrays(1, &VAO_FrameBuff);
		glGenBuffers(1, &VBO_FrameBuff);
		// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
		glBindVertexArray(VAO_FrameBuff);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_FrameBuff);
		glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

		GLint posAttrib = glGetAttribLocation(framebufferShader->Program, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

		GLint texAttrib = glGetAttribLocation(framebufferShader->Program, "texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

		glBindVertexArray(0); // Unbind VAO
	}

	// Load and create a texture
	// --== TEXTURE 1 == --
	//GLuint texture1;
	{
		int width, height;
		unsigned char* image;
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
												// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load, create texture and generate mipmaps
		image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	}

	// --== TEXTURE 2 == --
	//GLuint texture2;
	{
		int width, height;
		unsigned char* image;
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load, create texture and generate mipmaps
		image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//--=Framebuffer=--
	//GLuint frameBuffer, texColorBuffer;
	GLuint rboDepthStencil;
	{
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		//Texture for framebuffer
		glGenTextures(1, &texColorBuffer);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
			);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0
			);
		glBindTexture(GL_TEXTURE_2D, 0);
		// Depth stencil for framebuffer
		glGenRenderbuffers(1, &rboDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil
			);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//--=Downsampled framebuffer=--
	//GLuint frameDownBuffer, texColorDownBuffer;
	GLuint rboDownDepthStencil;
	{
		glGenFramebuffers(1, &frameDownBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameDownBuffer);
		//Downsampled Texture for framebuffer
		glGenTextures(1, &texColorDownBuffer);
		glBindTexture(GL_TEXTURE_2D, texColorDownBuffer);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA, screenWidth / FontX, screenHeight / FontY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
			);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorDownBuffer, 0
			);
		glBindTexture(GL_TEXTURE_2D, 0);
		// Depth stencil for framebuffer
		glGenRenderbuffers(1, &rboDownDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDownDepthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth / FontX, screenHeight / FontY);
		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDownDepthStencil
			);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	fontSize = fontChar.size();
	//--=ASCII as texture=--
	//GLuint ASCIITexture;
	{
		glGenTextures(1, &ASCIITexture);
		glBindTexture(GL_TEXTURE_2D, ASCIITexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FontX, FontY*fontSize, 0, GL_RED, GL_UNSIGNED_BYTE, fontChar.ASCIIChar.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//--=ASCII Brightness as texture=--
	//GLuint ASCIIBrightness;
	{
		glGenTextures(1, &ASCIIBrightness);
		glBindTexture(GL_TEXTURE_2D, ASCIIBrightness);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fontChar.ASCIIBrightness.size(), 1, 0, GL_RED, GL_UNSIGNED_BYTE, fontChar.ASCIIBrightness.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//--=ASCII Weights as texture=--
	asciiScreenWeights = new float[fontSize * screenWidth / FontX * screenHeight / FontY];
	//GLuint ASCIIScreenWeights;
	{
		glGenTextures(1, &ASCIIScreenWeights);
		glBindTexture(GL_TEXTURE_2D, ASCIIScreenWeights);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, fontSize / 4 * screenWidth / FontX, screenHeight / FontY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//--=ASCII Max Indexes as texture=--
	asciiMaxIndexes = new float[screenWidth / FontX * screenHeight / FontY];
	//GLuint ASCIIMaxIndexes;
	{
		glGenTextures(1, &ASCIIMaxIndexes);
		glBindTexture(GL_TEXTURE_2D, ASCIIMaxIndexes);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, screenWidth / FontX, screenHeight / FontY, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void LoadFonts() {
	// Load truetype fonts directly.
	fontNormal = fonsAddFont(fs, "sans", "DroidSerif-Regular.ttf");
	if (fontNormal == FONS_INVALID) {
		printf("Could not add font normal.\n");
		return;
	}
	fontItalic = fonsAddFont(fs, "sans-italic", "DroidSerif-Italic.ttf");
	if (fontItalic == FONS_INVALID) {
		printf("Could not add font italic.\n");
		return;
	}
	fontBold = fonsAddFont(fs, "sans-bold", "DroidSerif-Bold.ttf");
	if (fontBold == FONS_INVALID) {
		printf("Could not add font bold.\n");
		return;
	}
	fontJapanese = fonsAddFont(fs, "sans-jp", "DroidSansJapanese.ttf");
	if (fontJapanese == FONS_INVALID) {
		printf("Could not add font japanese.\n");
		return;
	}
}

void DrawDash(float dx, float dy)
{
	glBegin(GL_LINES);
	glColor4ub(0, 0, 0, 128);
	glVertex2f(dx - 5, dy);
	glVertex2f(dx - 10, dy);
	glEnd();
}

void DrawLine(float sx, float sy, float ex, float ey)
{
	glBegin(GL_LINES);
	glColor4ub(0, 0, 0, 128);
	glVertex2f(sx, sy);
	glVertex2f(ex, ey);
	glEnd();
}

/// Draws a black recangle, left-lower vertex: (x0,y0), right-upper vertex: (x1,y1)
void DrawBox(GLint x0, GLint y0, GLint x1, GLint y1)
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(x0, y0, x1 - x0, y1 - y0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}

void ClaculateVertices() {
	int gcdiv = gcd(screenWidth, screenHeight);
	GLfloat width = (GLfloat)screenWidth / (GLfloat)(gcdiv*2.0);
	GLfloat height = (GLfloat)screenHeight / (GLfloat)(gcdiv*2.0);
	GLfloat depth = (GLfloat)screenWidth / gcdiv;
	for (int i = 0; i < CubeVertices::CubeSides; i++) {
		// 6 points for 1 side and 5 parameters for 1 point
		for (int j = 0; j < 6; j++) {
			CubeVertices::index((6 * i + j) * 5 + 0) *= width;  // X
			CubeVertices::index((6 * i + j) * 5 + 1) *= height; // Y
			CubeVertices::index((6 * i + j) * 5 + 2) *= depth;  // Z
		}
	}
	const double rad = glm::radians((double)ZOOM / 2);
	const double proportionalConst = 8 / (450 / tan(rad));
	cubePositions[0] = glm::vec3(0.0f, 0.0f, -proportionalConst*screenHeight / (2 * tan(rad)));
}

int gcd(int a, int b) {
	if (b == 0)
		return a;
	return gcd(b, a % b);
}

void InitSideVAOVBO(GLuint &VAO, GLuint &VBO, GLfloat(*vertices)[CubeVertices::ArrSize]) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices), *vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}