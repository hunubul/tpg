#include "Source.h"
#include <map>
#include <vector>
#include <fstream>
#include "include\SOIL.h"
#include "OpenGLRender.h"

unsigned char* terminal;

int FontX = 8;
int FontY = 8;
FontChar fontChar;

FontChar::FontChar() {
	int width, height;
	terminal = SOIL_load_image("terminal.png", &width, &height, 0, SOIL_LOAD_RGB);
	CHAR::CHAR index = (CHAR::CHAR)0;
	std::vector<bool> tempFont;
	for (int i = 0; i < width / FontX; i++) {
		for (int j = 0; j < height / FontY; j++) {
			for (int k = 0; k < FontY; k++) {
				for (int l = 0; l < FontX; l++) {
					if (terminal[((j*FontY + k)*width + i*FontX + l) * 3] == 255)
						//White pixel
						tempFont.push_back(1);
					else
						//Black pixel
						tempFont.push_back(0);
				}
				ASCIIFont[index].push_back(tempFont);
				tempFont.clear();
			}
			for (int m = ASCIIFont[index].size()-1;m>=0;m--) {
				for (int n = 0; n<ASCIIFont[index][m].size(); n++) {
					ASCIIChar.push_back(ASCIIFont[index][m][n]*255);
				}
			}
			index = (CHAR::CHAR)(index + 1);
		}
	}
	SOIL_free_image_data(terminal);

	int brightness = 0;
	unsigned short brightHistogram = 0;
	for (unsigned int k = 0; k < ASCIIFont.size(); k++) {
		for (int i = 0; i < FontY; i++) {
			for (int j = 0; j < FontX; j++) {
				if (ASCIIFont[(CHAR::CHAR)k][i][j] == 1) {
					brightness++;
					brightHistogram += (j + 1) * 255;
				}
			}
			ASCIIHistogram[(CHAR::CHAR)k].push_back(brightHistogram);
			brightHistogram = 0;
		}
		ASCIIBrightness[(CHAR::CHAR)k] = brightness * 255 / (FontX*FontY);
		brightness = 0;
	}
}

int FontChar::size() {
	return ASCIIFont.size();
}

int main() {

	initAll();
	return 0;
}