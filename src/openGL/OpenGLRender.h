#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H

// OpenGL headers
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

// SDL headers
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>

// GL includes
#include "Camera.h"
#include "freetype-gl.h"
#include "vertex-buffer.h"
#include "text-buffer.h"
#include "mat4.h"

// Function prototypes
void Do_Movement();
void frameLimiter();
void render3Dmodels();
void RenderSubsection(int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight);
void setDirtyParams();
void drawBufferShader();
void DrawTextGL();
void DrawUIGL();

extern SDL_Event sdlEvent;
extern Camera camera;
extern bool keys[1024];

extern bool quit;
extern bool ASCIION;
extern bool firstRender;
extern bool newRoom;

extern GLfloat deltaTime;
extern GLfloat lastFrame;

extern std::vector<int> dirtyX;
extern std::vector<int> dirtyY;
extern std::vector<int> dirtyWidth;
extern std::vector<int> dirtyHeight;

#endif