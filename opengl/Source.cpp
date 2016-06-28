#include "Source.h"
#include <map>
#include <vector>
#include <fstream>
#include "include\SOIL.h"
#include "OpenGLRender.h"

unsigned char* terminal;

const int FontX = 8;
const int FontY = 8;
const int WriteOutFontX = 8;
const int WriteOutFontY = 12;
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
		ASCIIBrightness.push_back(brightness * 255 / (FontX*FontY));
		brightness = 0;
	}

	GenASCIIFontWriteOut();
}

int FontChar::size() {
	return ASCIIFont.size();
}

inline std::vector<std::vector<bool>> ReturnBoolArray(unsigned char in[WriteOutFontY], int maxbit) {
	std::vector<std::vector<bool>> ret;
	std::vector<bool> line;
	unsigned char shifted;
	for (int y = 0; y<WriteOutFontY; y++) {
		for (int x = 0;x<WriteOutFontX;x++) {
			shifted = in[y] << x;
			line.push_back( floor(shifted /maxbit) );
		}
		ret.push_back(line);
		line.clear();
	}
	return ret;
}

void FontChar::GenASCIIFontWriteOut() {
	/*
	--------
	-###----
	##-##---
	##-##---
	-###----
	#####-#-
	##-####-
	##--##--
	##-###--
	-###-##-
	--------
	--------

	00000000
	01110000
	11011000
	11011000
	01110000
	11111010
	11011110
	11001100
	11011100
	01110110
	00000000
	00000000

	//Broken up into 4 8x3 (24 bit) chunks for each component of the vec4.
	//Hexadecimal is being used to reduce clutter in the code but decimal still works.

	00000000
	01110000 -> 00000000 01110000 11011000 -> 0x0070D8
	11011000

	11011000
	01110000 -> 11011000 01110000 11111010 -> 0xD870FA
	11111010

	11011110
	11001100 -> 11011110 11001100 11011100 -> 0xDECCDC
	11011100

	01110110
	00000000 -> 01110110 00000000 00000000 -> 0x760000
	00000000

	vec4(0x0070D8,0xD870FA,0xDECCDC,0x760000)
	*/

	//Automatically generated from the 8x12 font sheet here:
	//http://www.massmind.org/techref/datafile/charset/extractor/charset_extractor.htm

	unsigned char ch_spc[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char ch_exc[WriteOutFontY] = { 0x00, 0x30, 0x78, 0x78, 0x78, 0x30, 0x30, 0x00, 0x30, 0x30, 0x00, 0x00 };
	unsigned char ch_quo[WriteOutFontY] = { 0x00, 0x66, 0x66, 0x66, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char ch_hsh[WriteOutFontY] = { 0x00, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0x00, 0x00 };
	unsigned char ch_dol[WriteOutFontY] = { 0x30, 0x30, 0x7C, 0xC0, 0xC0, 0x78, 0x0C, 0x0C, 0xF8, 0x30, 0x30, 0x00 };
	unsigned char ch_pct[WriteOutFontY] = { 0x00, 0x00, 0x00, 0xC4, 0xCC, 0x18, 0x30, 0x60, 0xCC, 0x8C, 0x00, 0x00 };
	unsigned char ch_amp[WriteOutFontY] = { 0x00, 0x70, 0xD8, 0xD8, 0x70, 0xFA, 0xDE, 0xCC, 0xDC, 0x76, 0x00, 0x00 };
	unsigned char ch_apo[WriteOutFontY] = { 0x00, 0x30, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char ch_lbr[WriteOutFontY] = { 0x00, 0x0C, 0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x0C, 0x00, 0x00 };
	unsigned char ch_rbr[WriteOutFontY] = { 0x00, 0x60, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00 };
	unsigned char ch_ast[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00 };
	unsigned char ch_crs[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00 };
	unsigned char ch_com[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x60, 0x00 };
	unsigned char ch_dsh[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char ch_per[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00 };
	unsigned char ch_lsl[WriteOutFontY] = { 0x00, 0x00, 0x02, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00, 0x00 };
	unsigned char ch_0[WriteOutFontY] = { 0x00, 0x7C, 0xC6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xC6, 0x7C, 0x00, 0x00 };
	unsigned char ch_1[WriteOutFontY] = { 0x00, 0x10, 0x30, 0xF0, 0x30, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00, 0x00 };
	unsigned char ch_2[WriteOutFontY] = { 0x00, 0x78, 0xCC, 0xCC, 0x0C, 0x18, 0x30, 0x60, 0xCC, 0xFC, 0x00, 0x00 };
	unsigned char ch_3[WriteOutFontY] = { 0x00, 0x78, 0xCC, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0xCC, 0x78, 0x00, 0x00 };
	unsigned char ch_4[WriteOutFontY] = { 0x00, 0x0C, 0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x0C, 0x1E, 0x00, 0x00 };
	unsigned char ch_5[WriteOutFontY] = { 0x00, 0xFC, 0xC0, 0xC0, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00, 0x00 };
	unsigned char ch_6[WriteOutFontY] = { 0x00, 0x38, 0x60, 0xC0, 0xC0, 0xF8, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x00 };
	unsigned char ch_7[WriteOutFontY] = { 0x00, 0xFE, 0xC6, 0xC6, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00, 0x00 };
	unsigned char ch_8[WriteOutFontY] = { 0x00, 0x78, 0xCC, 0xCC, 0xEC, 0x78, 0xDC, 0xCC, 0xCC, 0x78, 0x00, 0x00 };
	unsigned char ch_9[WriteOutFontY] = { 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x7C, 0x18, 0x18, 0x30, 0x70, 0x00, 0x00 };
	unsigned char ch_col[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00 };
	unsigned char ch_scl[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x18, 0x30, 0x00 };
	unsigned char ch_les[WriteOutFontY] = { 0x00, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x00, 0x00 };
	unsigned char ch_equ[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char ch_grt[WriteOutFontY] = { 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00 };
	unsigned char ch_que[WriteOutFontY] = { 0x00, 0x78, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x00, 0x30, 0x30, 0x00, 0x00 };
	unsigned char ch_ats[WriteOutFontY] = { 0x00, 0x7C, 0xC6, 0xC6, 0xDE, 0xDE, 0xDE, 0xC0, 0xC0, 0x7C, 0x00, 0x00 };
	unsigned char ch_A[WriteOutFontY] = { 0x00, 0x30, 0x78, 0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00, 0x00 };
	unsigned char ch_B[WriteOutFontY] = { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00 };
	unsigned char ch_C[WriteOutFontY] = { 0x00, 0x3C, 0x66, 0xC6, 0xC0, 0xC0, 0xC0, 0xC6, 0x66, 0x3C, 0x00, 0x00 };
	unsigned char ch_D[WriteOutFontY] = { 0x00, 0xF8, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, 0x00 };
	unsigned char ch_E[WriteOutFontY] = { 0x00, 0xFE, 0x62, 0x60, 0x64, 0x7C, 0x64, 0x60, 0x62, 0xFE, 0x00, 0x00 };
	unsigned char ch_F[WriteOutFontY] = { 0x00, 0xFE, 0x66, 0x62, 0x64, 0x7C, 0x64, 0x60, 0x60, 0xF0, 0x00, 0x00 };
	unsigned char ch_G[WriteOutFontY] = { 0x00, 0x3C, 0x66, 0xC6, 0xC0, 0xC0, 0xCE, 0xC6, 0x66, 0x3E, 0x00, 0x00 };
	unsigned char ch_H[WriteOutFontY] = { 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x00 };
	unsigned char ch_I[WriteOutFontY] = { 0x00, 0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00 };
	unsigned char ch_J[WriteOutFontY] = { 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x00 };
	unsigned char ch_K[WriteOutFontY] = { 0x00, 0xE6, 0x66, 0x6C, 0x6C, 0x78, 0x6C, 0x6C, 0x66, 0xE6, 0x00, 0x00 };
	unsigned char ch_L[WriteOutFontY] = { 0x00, 0xF0, 0x60, 0x60, 0x60, 0x60, 0x62, 0x66, 0x66, 0xFE, 0x00, 0x00 };
	unsigned char ch_M[WriteOutFontY] = { 0x00, 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00 };
	unsigned char ch_N[WriteOutFontY] = { 0x00, 0xC6, 0xC6, 0xE6, 0xF6, 0xFE, 0xDE, 0xCE, 0xC6, 0xC6, 0x00, 0x00 };
	unsigned char ch_O[WriteOutFontY] = { 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00, 0x00 };
	unsigned char ch_P[WriteOutFontY] = { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00 };
	unsigned char ch_Q[WriteOutFontY] = { 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0xCE, 0xDE, 0x7C, 0x0C, 0x1E, 0x00 };
	unsigned char ch_R[WriteOutFontY] = { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0x66, 0xE6, 0x00, 0x00 };
	unsigned char ch_S[WriteOutFontY] = { 0x00, 0x78, 0xCC, 0xCC, 0xC0, 0x70, 0x18, 0xCC, 0xCC, 0x78, 0x00, 0x00 };
	unsigned char ch_T[WriteOutFontY] = { 0x00, 0xFC, 0xB4, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00 };
	unsigned char ch_U[WriteOutFontY] = { 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x00 };
	unsigned char ch_V[WriteOutFontY] = { 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00, 0x00 };
	unsigned char ch_W[WriteOutFontY] = { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xD6, 0x6C, 0x6C, 0x6C, 0x00, 0x00 };
	unsigned char ch_X[WriteOutFontY] = { 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x78, 0xCC, 0xCC, 0xCC, 0x00, 0x00 };
	unsigned char ch_Y[WriteOutFontY] = { 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00 };
	unsigned char ch_Z[WriteOutFontY] = { 0x00, 0xFE, 0xCE, 0x98, 0x18, 0x30, 0x60, 0x62, 0xC6, 0xFE, 0x00, 0x00 };
	unsigned char ch_lsb[WriteOutFontY] = { 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, 0x00 };
	unsigned char ch_rsl[WriteOutFontY] = { 0x00, 0x00, 0x80, 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00, 0x00 };
	unsigned char ch_rsb[WriteOutFontY] = { 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00, 0x00 };
	unsigned char ch_pow[WriteOutFontY] = { 0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char ch_usc[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00 };
	unsigned char ch_a[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0x00, 0x00 };
	unsigned char ch_b[WriteOutFontY] = { 0x00, 0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xDC, 0x00, 0x00 };
	unsigned char ch_c[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x78, 0xCC, 0xC0, 0xC0, 0xCC, 0x78, 0x00, 0x00 };
	unsigned char ch_d[WriteOutFontY] = { 0x00, 0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00 };
	unsigned char ch_e[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0xCC, 0x78, 0x00, 0x00 };
	unsigned char ch_f[WriteOutFontY] = { 0x00, 0x38, 0x6C, 0x60, 0x60, 0xF8, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00 };
	unsigned char ch_g[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xCC, 0x78 };
	unsigned char ch_h[WriteOutFontY] = { 0x00, 0xE0, 0x60, 0x60, 0x6C, 0x76, 0x66, 0x66, 0x66, 0xE6, 0x00, 0x00 };
	unsigned char ch_i[WriteOutFontY] = { 0x00, 0x18, 0x18, 0x00, 0x78, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00 };
	unsigned char ch_j[WriteOutFontY] = { 0x00, 0x0C, 0x0C, 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78 };
	unsigned char ch_k[WriteOutFontY] = { 0x00, 0xE0, 0x60, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00, 0x00 };
	unsigned char ch_l[WriteOutFontY] = { 0x00, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00 };
	unsigned char ch_m[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0xFC, 0xD6, 0xD6, 0xD6, 0xD6, 0xC6, 0x00, 0x00 };
	unsigned char ch_n[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0xF8, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x00 };
	unsigned char ch_o[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x00 };
	unsigned char ch_p[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xF0 };
	unsigned char ch_q[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E };
	unsigned char ch_r[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0xEC, 0x6E, 0x76, 0x60, 0x60, 0xF0, 0x00, 0x00 };
	unsigned char ch_s[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x78, 0xCC, 0x60, 0x18, 0xCC, 0x78, 0x00, 0x00 };
	unsigned char ch_t[WriteOutFontY] = { 0x00, 0x00, 0x20, 0x60, 0xFC, 0x60, 0x60, 0x60, 0x6C, 0x38, 0x00, 0x00 };
	unsigned char ch_u[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00 };
	unsigned char ch_v[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00, 0x00 };
	unsigned char ch_w[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xD6, 0x6C, 0x6C, 0x00, 0x00 };
	unsigned char ch_x[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0xC6, 0x6C, 0x38, 0x38, 0x6C, 0xC6, 0x00, 0x00 };
	unsigned char ch_y[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x0C, 0x18, 0xF0 };
	unsigned char ch_z[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x00, 0xFC, 0x8C, 0x18, 0x60, 0xC4, 0xFC, 0x00, 0x00 };
	unsigned char ch_lpa[WriteOutFontY] = { 0x00, 0x1C, 0x30, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x30, 0x1C, 0x00, 0x00 };
	unsigned char ch_bar[WriteOutFontY] = { 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00 };
	unsigned char ch_rpa[WriteOutFontY] = { 0x00, 0xE0, 0x30, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x30, 0xE0, 0x00, 0x00 };
	unsigned char ch_tid[WriteOutFontY] = { 0x00, 0x73, 0xDA, 0xCE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char ch_lar[WriteOutFontY] = { 0x00, 0x00, 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00, 0x00, 0x00 };

	int maxbit = pow(2, WriteOutFontX - 1);
	ASCIIFontWriteOut[CHAR::SPACE] = ReturnBoolArray(ch_spc, maxbit);
	ASCIIFontWriteOut[CHAR::EXCLAMATION] = ReturnBoolArray(ch_exc, maxbit);
	ASCIIFontWriteOut[CHAR::DOUBLE_QUOTE] = ReturnBoolArray(ch_quo, maxbit);
	ASCIIFontWriteOut[CHAR::HASHMARK] = ReturnBoolArray(ch_hsh, maxbit);
	ASCIIFontWriteOut[CHAR::DOLLAR] = ReturnBoolArray(ch_dol, maxbit);
	ASCIIFontWriteOut[CHAR::PERCENT] = ReturnBoolArray(ch_pct, maxbit);
	ASCIIFontWriteOut[CHAR::AND] = ReturnBoolArray(ch_amp, maxbit);
	ASCIIFontWriteOut[CHAR::APOSTROPHE] = ReturnBoolArray(ch_apo, maxbit);
	ASCIIFontWriteOut[CHAR::BRACKET_LEFT] = ReturnBoolArray(ch_lbr, maxbit);
	ASCIIFontWriteOut[CHAR::BRACKET_RIGHT] = ReturnBoolArray(ch_rbr, maxbit);
	ASCIIFontWriteOut[CHAR::ASTERIX] = ReturnBoolArray(ch_ast, maxbit);
	ASCIIFontWriteOut[CHAR::PLUS] = ReturnBoolArray(ch_crs, maxbit);
	ASCIIFontWriteOut[CHAR::COMMA] = ReturnBoolArray(ch_com, maxbit);
	ASCIIFontWriteOut[CHAR::DASH] = ReturnBoolArray(ch_dsh, maxbit);
	ASCIIFontWriteOut[CHAR::SLASH] = ReturnBoolArray(ch_per, maxbit);
	ASCIIFontWriteOut[CHAR::PERIOD] = ReturnBoolArray(ch_lsl, maxbit);
	ASCIIFontWriteOut[CHAR::NUM0] = ReturnBoolArray(ch_0, maxbit);
	ASCIIFontWriteOut[CHAR::NUM1] = ReturnBoolArray(ch_1, maxbit);
	ASCIIFontWriteOut[CHAR::NUM2] = ReturnBoolArray(ch_2, maxbit);
	ASCIIFontWriteOut[CHAR::NUM3] = ReturnBoolArray(ch_3, maxbit);
	ASCIIFontWriteOut[CHAR::NUM4] = ReturnBoolArray(ch_4, maxbit);
	ASCIIFontWriteOut[CHAR::NUM5] = ReturnBoolArray(ch_5, maxbit);
	ASCIIFontWriteOut[CHAR::NUM6] = ReturnBoolArray(ch_6, maxbit);
	ASCIIFontWriteOut[CHAR::NUM7] = ReturnBoolArray(ch_7, maxbit);
	ASCIIFontWriteOut[CHAR::NUM8] = ReturnBoolArray(ch_8, maxbit);
	ASCIIFontWriteOut[CHAR::NUM9] = ReturnBoolArray(ch_9, maxbit);
	ASCIIFontWriteOut[CHAR::COLON] = ReturnBoolArray(ch_col, maxbit);
	ASCIIFontWriteOut[CHAR::SEMICOLON] = ReturnBoolArray(ch_scl, maxbit);
	ASCIIFontWriteOut[CHAR::LESS_THAN] = ReturnBoolArray(ch_les, maxbit);
	ASCIIFontWriteOut[CHAR::EQUAL] = ReturnBoolArray(ch_equ, maxbit);
	ASCIIFontWriteOut[CHAR::GREATER_THAN] = ReturnBoolArray(ch_grt, maxbit);
	ASCIIFontWriteOut[CHAR::QUESTION] = ReturnBoolArray(ch_que, maxbit);
	ASCIIFontWriteOut[CHAR::AT] = ReturnBoolArray(ch_ats, maxbit);
	ASCIIFontWriteOut[CHAR::A] = ReturnBoolArray(ch_A, maxbit);
	ASCIIFontWriteOut[CHAR::B] = ReturnBoolArray(ch_B, maxbit);
	ASCIIFontWriteOut[CHAR::C] = ReturnBoolArray(ch_C, maxbit);
	ASCIIFontWriteOut[CHAR::D] = ReturnBoolArray(ch_D, maxbit);
	ASCIIFontWriteOut[CHAR::E] = ReturnBoolArray(ch_E, maxbit);
	ASCIIFontWriteOut[CHAR::F] = ReturnBoolArray(ch_F, maxbit);
	ASCIIFontWriteOut[CHAR::G] = ReturnBoolArray(ch_G, maxbit);
	ASCIIFontWriteOut[CHAR::H] = ReturnBoolArray(ch_H, maxbit);
	ASCIIFontWriteOut[CHAR::I] = ReturnBoolArray(ch_I, maxbit);
	ASCIIFontWriteOut[CHAR::J] = ReturnBoolArray(ch_J, maxbit);
	ASCIIFontWriteOut[CHAR::K] = ReturnBoolArray(ch_K, maxbit);
	ASCIIFontWriteOut[CHAR::L] = ReturnBoolArray(ch_L, maxbit);
	ASCIIFontWriteOut[CHAR::M] = ReturnBoolArray(ch_M, maxbit);
	ASCIIFontWriteOut[CHAR::N] = ReturnBoolArray(ch_N, maxbit);
	ASCIIFontWriteOut[CHAR::O] = ReturnBoolArray(ch_O, maxbit);
	ASCIIFontWriteOut[CHAR::P] = ReturnBoolArray(ch_P, maxbit);
	ASCIIFontWriteOut[CHAR::Q] = ReturnBoolArray(ch_Q, maxbit);
	ASCIIFontWriteOut[CHAR::R] = ReturnBoolArray(ch_R, maxbit);
	ASCIIFontWriteOut[CHAR::S] = ReturnBoolArray(ch_S, maxbit);
	ASCIIFontWriteOut[CHAR::T] = ReturnBoolArray(ch_T, maxbit);
	ASCIIFontWriteOut[CHAR::U] = ReturnBoolArray(ch_U, maxbit);
	ASCIIFontWriteOut[CHAR::V] = ReturnBoolArray(ch_V, maxbit);
	ASCIIFontWriteOut[CHAR::W] = ReturnBoolArray(ch_W, maxbit);
	ASCIIFontWriteOut[CHAR::X] = ReturnBoolArray(ch_X, maxbit);
	ASCIIFontWriteOut[CHAR::Y] = ReturnBoolArray(ch_Y, maxbit);
	ASCIIFontWriteOut[CHAR::Z] = ReturnBoolArray(ch_Z, maxbit);
	ASCIIFontWriteOut[CHAR::SQUARE_BRACKET_LEFT] = ReturnBoolArray(ch_lsb, maxbit);
	ASCIIFontWriteOut[CHAR::BACKSLASH] = ReturnBoolArray(ch_rsl, maxbit);
	ASCIIFontWriteOut[CHAR::SQUARE_BRACKET_RIGHT] = ReturnBoolArray(ch_rsb, maxbit);
	ASCIIFontWriteOut[CHAR::ROOF] = ReturnBoolArray(ch_pow, maxbit);
	ASCIIFontWriteOut[CHAR::UNDERSCORE] = ReturnBoolArray(ch_usc, maxbit);
	ASCIIFontWriteOut[CHAR::a] = ReturnBoolArray(ch_a, maxbit);
	ASCIIFontWriteOut[CHAR::b] = ReturnBoolArray(ch_b, maxbit);
	ASCIIFontWriteOut[CHAR::c] = ReturnBoolArray(ch_c, maxbit);
	ASCIIFontWriteOut[CHAR::d] = ReturnBoolArray(ch_d, maxbit);
	ASCIIFontWriteOut[CHAR::e] = ReturnBoolArray(ch_e, maxbit);
	ASCIIFontWriteOut[CHAR::f] = ReturnBoolArray(ch_f, maxbit);
	ASCIIFontWriteOut[CHAR::g] = ReturnBoolArray(ch_g, maxbit);
	ASCIIFontWriteOut[CHAR::h] = ReturnBoolArray(ch_h, maxbit);
	ASCIIFontWriteOut[CHAR::i] = ReturnBoolArray(ch_i, maxbit);
	ASCIIFontWriteOut[CHAR::j] = ReturnBoolArray(ch_j, maxbit);
	ASCIIFontWriteOut[CHAR::k] = ReturnBoolArray(ch_k, maxbit);
	ASCIIFontWriteOut[CHAR::l] = ReturnBoolArray(ch_l, maxbit);
	ASCIIFontWriteOut[CHAR::m] = ReturnBoolArray(ch_m, maxbit);
	ASCIIFontWriteOut[CHAR::n] = ReturnBoolArray(ch_n, maxbit);
	ASCIIFontWriteOut[CHAR::o] = ReturnBoolArray(ch_o, maxbit);
	ASCIIFontWriteOut[CHAR::p] = ReturnBoolArray(ch_p, maxbit);
	ASCIIFontWriteOut[CHAR::q] = ReturnBoolArray(ch_q, maxbit);
	ASCIIFontWriteOut[CHAR::r] = ReturnBoolArray(ch_r, maxbit);
	ASCIIFontWriteOut[CHAR::s] = ReturnBoolArray(ch_s, maxbit);
	ASCIIFontWriteOut[CHAR::t] = ReturnBoolArray(ch_t, maxbit);
	ASCIIFontWriteOut[CHAR::u] = ReturnBoolArray(ch_u, maxbit);
	ASCIIFontWriteOut[CHAR::v] = ReturnBoolArray(ch_v, maxbit);
	ASCIIFontWriteOut[CHAR::w] = ReturnBoolArray(ch_w, maxbit);
	ASCIIFontWriteOut[CHAR::x] = ReturnBoolArray(ch_x, maxbit);
	ASCIIFontWriteOut[CHAR::y] = ReturnBoolArray(ch_y, maxbit);
	ASCIIFontWriteOut[CHAR::z] = ReturnBoolArray(ch_z, maxbit);
	ASCIIFontWriteOut[CHAR::BRACKETS_CURLY_LEFT] = ReturnBoolArray(ch_lpa, maxbit);
	ASCIIFontWriteOut[CHAR::VERTICAL_BAR] = ReturnBoolArray(ch_bar, maxbit);
	ASCIIFontWriteOut[CHAR::BRACKETS_CURLY_RIGHT] = ReturnBoolArray(ch_rpa, maxbit);
	ASCIIFontWriteOut[CHAR::TILDE] = ReturnBoolArray(ch_tid, maxbit);
	ASCIIFontWriteOut[CHAR::HOUSE] = ReturnBoolArray(ch_lar, maxbit);
}

int main() {

	initAll();
	return 0;
}