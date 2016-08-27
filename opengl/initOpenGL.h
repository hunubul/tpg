#pragma once
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

void initOpenGL();

//Font functions
void DrawDash(float dx, float dy);
void DrawLine(float sx, float sy, float ex, float ey);
void DrawBox(GLint x0, GLint y0, GLint x1, GLint y1);

// extern variables
extern SDL_Window* window;
extern const GLuint screenWidth, screenHeight;

extern int fontNormal, fontItalic, fontBold, fontJapanese;
extern FONScontext* fs;

extern GLuint VAO_FrameBuff, VBO_FrameBuff;
extern GLuint VAOfront, VBOfront;
extern GLuint VAOleft, VBOleft;
extern GLuint VAOright, VBOright;
extern GLuint VAOfloor, VBOfloor;
extern GLuint VAOceiling, VBOceiling;

extern GLuint texture1, texture2;
extern GLuint frameBuffer, texColorBuffer;
extern GLuint frameDownBuffer, texColorDownBuffer;
extern GLuint ASCIITexture, ASCIIBrightness, ASCIIScreenWeights, ASCIIMaxIndexes;

extern float *asciiScreenWeights, *asciiMaxIndexes;
extern int fontSize;

// Shaders
extern Shader* defaultShader;
extern Shader* framebufferShader;
extern ComputeShader* computeASCIIShader;
extern ComputeShader* computeASCIIMaxIndexShader;