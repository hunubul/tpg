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

class Font {
private:
	int idx;
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

	void NewLine();
};

// Text Vector for writing to screen
enum TEXT_TYPE { MINIMAP_TEXT=0, ARROW_TEXT };
extern std::map<TEXT_TYPE, Font> texts;

// Function prototypes
void Do_Movement();
void frameLimiter();
void render3Dmodels();
void RenderSubsection(int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight);
void setDirtyParams();
void drawBufferShader();
void DrawTextGL();

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