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
#include "ASCII.h"
#include <codecvt>

// GL includes
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
SDL_Event sdlEvent;

// Properties
const GLuint screenWidth = 1600, screenHeight = 900;
bool ASCIION = true;
bool quit = false;

//Shaders location
std::string texturesVertPath = "./shaders/textures.vert";
std::string texturesFragPath = "./shaders/textures.frag";
std::string framebufferVertPath = "./shaders/framebuffer.vert";
std::string framebufferFragPath = "./shaders/framebuffer.frag";
std::string computeASCIIShaderPath = "./shaders/computeASCII.shd";
std::string computeASCIIMaxIndexShaderPath = "./shaders/computeASCIIMaxIndex.shd";

// Function prototypes
void Do_Movement();
void ManageEvents();

void LoadFonts();
void DrawText();
void DrawDash(float dx, float dy);
void DrawLine(float sx, float sy, float ex, float ey);
void DrawBox(GLint x0, GLint y0, GLint x1, GLint y1);
FONScontext* fs = NULL;
int fontNormal, fontItalic, fontBold, fontJapanese;

int gcd(int a, int b); //Greatest Common Divisor
void ClaculateVertices();
void InitSideVAOVBO(GLuint &VAO, GLuint &VBO, GLfloat (*vertices)[CubeVertices::ArrSize]);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
void initAll()
{
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
	std::vector<int> computeParams = { (int)fontChar.size() };
	Shader defaultShader(texturesVertPath, texturesFragPath, std::vector<int>() );
	Shader framebufferShader(framebufferVertPath, framebufferFragPath, fragmentParams);
	ComputeShader computeASCIIShader(computeASCIIShaderPath, computeParams);
	ComputeShader computeASCIIMaxIndexShader(computeASCIIMaxIndexShaderPath, std::vector<int>() );

	// Fonts generation
	fs = gl3fonsCreate(512, 512, FONS_ZERO_TOPLEFT);
	if (!fs) {
		fprintf(stderr, "Could not create stash.\n");
		return;
	}
	LoadFonts();

	ClaculateVertices();

	GLuint VAOfront, VBOfront;
	GLuint VAOleft, VBOleft;
	GLuint VAOright, VBOright;
	GLuint VAOfloor, VBOfloor;
	GLuint VAOceiling, VBOceiling;
	InitSideVAOVBO(VAOfront, VBOfront, &CubeVertices::Front);
	InitSideVAOVBO(VAOleft, VBOleft, &CubeVertices::Left);
	InitSideVAOVBO(VAOright, VBOright, &CubeVertices::Right);
	InitSideVAOVBO(VAOfloor, VBOfloor, &CubeVertices::Floor);
	InitSideVAOVBO(VAOceiling, VBOceiling, &CubeVertices::Ceiling);

	GLuint VAO_FrameBuff, VBO_FrameBuff;
	{
		glGenVertexArrays(1, &VAO_FrameBuff);
		glGenBuffers(1, &VBO_FrameBuff);
		// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
		glBindVertexArray(VAO_FrameBuff);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_FrameBuff);
		glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

		GLint posAttrib = glGetAttribLocation(framebufferShader.Program, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

		GLint texAttrib = glGetAttribLocation(framebufferShader.Program, "texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

		glBindVertexArray(0); // Unbind VAO
	}

	// Load and create a texture
	// --== TEXTURE 1 == --
	GLuint texture1;
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
	GLuint texture2;
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
	GLuint frameBuffer, texColorBuffer, rboDepthStencil;
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
	GLuint frameDownBuffer, texColorDownBuffer, rboDownDepthStencil;
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

	int fontSize = fontChar.size();
	//--=ASCII as texture=--
	GLuint ASCIITexture;
	{
		glGenTextures(1, &ASCIITexture);
		glBindTexture(GL_TEXTURE_2D, ASCIITexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FontX, FontY*fontSize, 0, GL_RED, GL_UNSIGNED_BYTE, fontChar.ASCIIChar.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//--=ASCII Brightness as texture=--
	GLuint ASCIIBrightness;
	{
		glGenTextures(1, &ASCIIBrightness);
		glBindTexture(GL_TEXTURE_2D, ASCIIBrightness);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fontChar.ASCIIBrightness.size(), 1, 0, GL_RED, GL_UNSIGNED_BYTE, fontChar.ASCIIBrightness.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//--=ASCII Weights as texture=--
	float* asciiScreenWeights = new float[fontSize * screenWidth / FontX * screenHeight / FontY];
	GLuint ASCIIScreenWeights;
	{
		glGenTextures(1, &ASCIIScreenWeights);
		glBindTexture(GL_TEXTURE_2D, ASCIIScreenWeights);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, fontSize/4 * screenWidth / FontX, screenHeight / FontY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//--=ASCII Max Indexes as texture=--
	float* asciiMaxIndexes = new float[screenWidth / FontX * screenHeight / FontY];
	GLuint ASCIIMaxIndexes;
	{
		glGenTextures(1, &ASCIIMaxIndexes);
		glBindTexture(GL_TEXTURE_2D, ASCIIMaxIndexes);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, screenWidth / FontX, screenHeight / FontY, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned char* texture1Arr = new unsigned char[screenWidth *screenHeight * 3];
	bool newRoom = true;

	// Game loop
	while (!quit) {
		ManageEvents();

		// Set frame time
		GLfloat currentFrame = (GLfloat)SDL_GetTicks() / 1000;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		Do_Movement();

		// Clear the colorbuffer
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw our first triangle
		defaultShader.UseProgram();

		//Change textures
		if(newRoom){
			glBindTexture(GL_TEXTURE_2D, texture1);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1Arr);
			glBindTexture(GL_TEXTURE_2D, 0);
			newRoom = false;
		}

		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(defaultShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(defaultShader.Program, "ourTexture2"), 1);

		// Create camera transformation
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(defaultShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(defaultShader.Program, "view");
		GLint projLoc = glGetUniformLocation(defaultShader.Program, "projection");
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

		/*GLenum err = GL_NO_ERROR;
		while ((err = glGetError()) != GL_NO_ERROR) {
			printf("Error: %x\n", err);
		}*/

		if (ASCIION) {
			int dirtyX = 0 / FontX;
			int dirtyY = 0 / FontY;
			int dirtyWidth = screenWidth / FontX;
			int dirtyHeight = screenHeight / FontY;

			computeASCIIShader.UseComputerShaderProgram();
			glBindImageTexture(0, texColorBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
			glBindImageTexture(1, texColorDownBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
			glBindImageTexture(2, ASCIITexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
			glBindImageTexture(3, ASCIIBrightness, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
			glBindImageTexture(4, ASCIIScreenWeights, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
			glUniform1i(0, (GLint)dirtyX);
			glUniform1i(1, (GLint)dirtyY);

			glDispatchCompute(1, dirtyWidth, dirtyHeight);

			glMemoryBarrier(GL_ALL_BARRIER_BITS);

			if (dirtyWidth*dirtyHeight <= 27*27) {
				//Faster to render using compute shader
				computeASCIIMaxIndexShader.UseComputerShaderProgram();
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


		glBindVertexArray(VAO_FrameBuff);

		framebufferShader.UseProgram();
		glBindImageTexture(0, texColorBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(1, ASCIITexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(2, ASCIIMaxIndexes, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16F);
		glUniform1i(0, (GLint)ASCIION);

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
				if (ASCIION) ASCIION = false;
				else         ASCIION = true;
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
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[SDLK_s])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[SDLK_a])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[SDLK_d])
		camera.ProcessKeyboard(RIGHT, deltaTime);
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
	glScissor(x0, y0, x1-x0, y1-y0);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
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
	utf8_string = convert.to_bytes( L"Ég get etið gler án þess að meiða mig." );
	fonsDrawText(fs, dx, dy, utf8_string.c_str(), NULL);

	fonsVertMetrics(fs, NULL, NULL, &lh);
	dx = sx;
	dy += lh*1.2f;
	DrawDash(dx, dy);
	fonsSetFont(fs, fontJapanese);
	utf8_string = convert.to_bytes( L"私はガラスを食べられます。それは私を傷つけません。" );
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

void ClaculateVertices() {
	int gcdiv  = gcd(screenWidth, screenHeight);
	GLfloat width  = (GLfloat)screenWidth  / (GLfloat)(gcdiv*2.0);
	GLfloat height = (GLfloat)screenHeight / (GLfloat)(gcdiv*2.0);
	GLfloat depth  = (GLfloat)screenWidth  / gcdiv;
	for (int i = 0; i < CubeVertices::CubeSides; i++) {
		// 6 points for 1 side and 5 parameters for 1 point
		for (int j = 0; j < 6; j++) {
			CubeVertices::index( (6 * i + j) * 5 + 0 ) *= width;  // X
			CubeVertices::index( (6 * i + j) * 5 + 1 ) *= height; // Y
			CubeVertices::index( (6 * i + j) * 5 + 2 ) *= depth;  // Z
		}
	}
	const double rad = glm::radians((double)ZOOM/2);
	const double proportionalConst = 8/( 450/tan(rad) );
	cubePositions[0] = glm::vec3( 0.0f, 0.0f, -proportionalConst*screenHeight/( 2*tan(rad) ) );
}

int gcd(int a, int b) {
	if (b == 0)
		return a;
	return gcd(b, a % b);
}

void InitSideVAOVBO(GLuint &VAO, GLuint &VBO, GLfloat (*vertices)[CubeVertices::ArrSize] ) {
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