#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H

// OpenGL headers
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

// SDL headers
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>

// GL includes
#include "Camera.h"
#include "fontstash.h"
#include "gl3fontstash.h"

class Font {
private:
	int idx;
public:
	struct textType {
		std::string text;
		unsigned int fontColor;
		bool newLine;
	};
	std::vector<textType> text;
	int fontType, fontAlign;
	float fontSize, fontSpacing, blurSize;
	float dx, dy;
	bool shadow;

	Font(int fontType) :
		fontType(fontType), fontAlign(FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE), idx(0),
		fontSize(36.f), fontSpacing(0.f), blurSize(0.f), dx(0.f), dy(0.f), shadow(false) {
		text.push_back({"", gl3fonsRGBA(255, 255, 255, 255), true});
	}
	Font(int fontType, unsigned char r, unsigned char g, unsigned char b) :
		fontType(fontType), fontAlign(FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE), idx(0),
		fontSize(36.f), fontSpacing(0.f), blurSize(0.f), dx(0.f), dy(0.f), shadow(false) {
		text.push_back({ "", gl3fonsRGBA(r, g, b, 255), true });
	}
	Font(int fontType, unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
		fontType(fontType), fontAlign(FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE), idx(0),
		fontSize(36.f), fontSpacing(0.f), blurSize(0.f), dx(0.f), dy(0.f), shadow(false) {
		text.push_back({ "", gl3fonsRGBA(r, g, b, a), true });
	}

	void changeFontColor(unsigned char r, unsigned char g, unsigned char b) {
		text[idx].newLine = false;
		text.push_back({ "", gl3fonsRGBA(r, g, b, 255), false });
		idx++;
	}
	void changeFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		text.push_back({ "", gl3fonsRGBA(r, g, b, a), false });
		idx++;
	}
	void append(std::string str) {
		text[idx].text.append(str);
	}
	void endLine() {
		text[idx].newLine = true;
		text.push_back({ "", text[idx].fontColor, true });
		idx++;
	}
};

// Text Vector for writing to screen
extern std::vector<Font> texts;

// Function prototypes
void Do_Movement();
void frameLimiter();
void render3Dmodels();
void RenderSubsection(int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight);
void setDirtyParams();
void drawBufferShader();
void DrawText();

extern SDL_Event sdlEvent;
extern Camera camera;
extern bool keys[1024];

extern bool quit;
extern bool ASCIION;
extern bool firstRender;

extern GLfloat deltaTime;
extern GLfloat lastFrame;

extern std::vector<int> dirtyX;
extern std::vector<int> dirtyY;
extern std::vector<int> dirtyWidth;
extern std::vector<int> dirtyHeight;

#endif