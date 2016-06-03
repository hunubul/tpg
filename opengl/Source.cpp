#include "Source.h"
#include <map>
#include <vector>
#include <fstream>
#include "include\SOIL.h"
#include "OpenGLRender.h"

unsigned char* terminal;

int FontX = 8;
int FontY = 8;
std::map<CHAR::CHAR, std::vector<bool> > FontChar;

int main() {
	int width, height;
	terminal = SOIL_load_image("terminal.png", &width, &height, 0, SOIL_LOAD_RGB);
	CHAR::CHAR index=(CHAR::CHAR)0;
	for (int i = 0; i < width / FontX; i++) {
		for (int j = 0; j < height / FontY; j++) {
			for (int k = 0; k < FontY; k++) {
				for (int l = 0; l < FontX; l++) {
					if(terminal[((j*FontY+k)*width + i*FontX+l)*3]==255)
						//White pixel
						FontChar[index].push_back(1);
					else
						//Black pixel
						FontChar[index].push_back(0);
				}
			}
			index = (CHAR::CHAR)(index + 1);
		}
	}
	SOIL_free_image_data(terminal);
	initAll();
	return 0;
}