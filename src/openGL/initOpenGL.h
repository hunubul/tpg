#pragma once
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
#include "freetype-gl.h"
#include "vertex-buffer.h"
#include "text-buffer.h"
#include "mat4.h"

void initOpenGL();

//Font functions
void DrawBox(GLint x0, GLint y0, GLint x1, GLint y1);
// Box starts from left-upper -> right-upper -> right-lower -> left-lower
template <class UIElement>
void FixUI_Vertices(const vec2 box[4]);

// extern variables
extern SDL_Window* window;
extern GLuint screenWidth, screenHeight;
extern const int MAX_NUM_OF_DIRTY_BLOCKS;

// Font variables
extern mat4   FontModelMat, FontViewMat, FontProjectionMat;
extern text_buffer_t* generalTextBuffer;
extern char* fontLuckiestGuyPath;
extern char* fontZigPath;

extern GLuint VAO_FrameBuff, VBO_FrameBuff;
extern GLuint VAOfront, VBOfront;
extern GLuint VAOleft, VBOleft;
extern GLuint VAOright, VBOright;
extern GLuint VAOfloor, VBOfloor;
extern GLuint VAOceiling, VBOceiling;

extern GLuint texture1, texture2;
extern GLuint frameBuffer, texColorBuffer;
extern GLuint frameDownBuffer, texColorDownBuffer;
extern GLuint prevframeTexture;
extern GLuint ASCIITexture, ASCIIBrightness, ASCIIScreenWeights, ASCIIMaxIndexes;

extern float *asciiScreenWeights, *asciiMaxIndexes;
extern unsigned char *downBuffer;
extern int fontSize;

extern bool initialisedGL;

// Shaders
extern Shader* defaultShader;
extern Shader* framebufferShader;
extern Shader* uiShader;
extern ComputeShader* computeASCIIShader;
extern ComputeShader* computeASCIIMaxIndexShader;