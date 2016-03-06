/**
 * @file Image2ASCIIcolors.cpp
 * @brief Capable of converting .PNG images to ASCII art with colors, uses Libtcod library
 * @author    Ákos Köte
 * @version   1.2
*/

#include "Image2ASCIIcolors.h"
#include <stdio.h>
#include "math.h"

/**
 * @brief Imports and arranges each ASCII CHAR
 * @param [out] CharSet CHAR_SET type, where the ASCII CHAR pixel informations are stored
 * @param [in] filename (8x14asciichars.dat) where the ASCII CHAR informations are, see [mainpage](index.html)
 */
void CharSetImporter(CHAR_SET*CharSet, const char* filename) {
	FILE *f;
	int i;
	char buff[9 + 2 * CHAR_X*CHAR_Y];
	f = fopen(filename, "r");
	if (f != NULL) {
		int char_index = 0;
		int char_line = 0;
		int char_column = 0;
		while (fgets(buff, 9 + 2 * CHAR_X*CHAR_Y, f) != NULL && char_index < CHAR_NUM) {
			for (i = 3; i < 9 + 2 * CHAR_X*CHAR_Y; i++) {
				if (buff[i] == '0' || buff[i] == '1') {
					CharSet->ARR[char_index][char_line][char_column] = buff[i];
					char_column++;
				}
				if ((i - 3) != 0 && (i - 3) % (CHAR_X * 2) == 0) {
					char_column = 0;
					char_line++;
				}
			}
			Indexer(CharSet, char_index);
			char_column = 0;
			char_line = 0;
			char_index++;
		}
	} else {
		TCODConsole::root->print(0, 0, "ERROR NOT FOUND %s", filename);
		TCODConsole::root->flush();
		TCODConsole::waitForKeypress(true);
		exit(1);
	}
	fclose(f);
}
/**
 * @brief Precise processing a .PNG image file
 * @param [out] PPM IMAGE type, gets the weight for each SUBSECTION
 * @param [in] subsec SUBSECTION informations
 * @param [in] CharSet CHAR_SET informations
 * @details Tries to contrast up SUBSECTIONS for clearer curves in images see AMPLIFIER, min((CHAR_SET.ARR[][][] - ImgSubWeight[][])^2)
 */
void PreciseProcessPNG(IMAGE* PNG, SUBSECTION& subsec, CHAR_SET& CharSet) {
	int WeightTemp[CHAR_NUM]; /* how many are similar */
	int ImgSubWeight[CHAR_Y][CHAR_X]; /* a pixel brightness of the subsection */
	int weight, minWeight, weightMax, weightMin, minIdx, cnt, cnt2;
	int R, G, B;
	PNG->ASCII_Image = (unsigned char*)calloc(PNG->WidthTile*PNG->HeightTile, sizeof(unsigned char));
	PNG->ASCII_Color = (TCODColor*)calloc(PNG->WidthTile*PNG->HeightTile, sizeof(TCODColor));
	double cmpSubsecY = subsec.height / CHAR_Y;
	double cmpSubsecX = subsec.width / CHAR_X;
	for (int pY = 0; pY < (int)PNG->HeightTile; pY++) {
		for (int pX = 0; pX < (int)PNG->WidthTile; pX++) {
			for (int m = 0; m < CHAR_NUM; m++) {
				WeightTemp[m] = 0;
			}
			weight = cnt = R = G = B = 0;
			minWeight = weightMax = weightMin = minIdx = cnt2 = 0;
			for (int cY = 0 + pY*CHAR_Y, k = 0; cY < CHAR_Y + pY*CHAR_Y; cY++, k++) {
				for (int cX = 0 + pX*CHAR_X, l = 0; cX < CHAR_X + pX*CHAR_X; cX++, l++) {
					for (double y = 0 + cY*cmpSubsecY; y < cmpSubsecY + cY*cmpSubsecY; y++) {
						for (double x = 0 + cX*cmpSubsecX; x < cmpSubsecX + cX*cmpSubsecX; x++) {
							cnt++;
							/* x: column, y: row, *4 RGBA, A: Alpha channel (visibility) 0->invisible, 255->fully visible */
							R += PNG->Image[((int)x + (int)y*(int)PNG->Width) * 4] /* R */
								* PNG->Image[((int)x + (int)y*(int)PNG->Width) * 4 + 3] / 255;
							G += PNG->Image[((int)x + (int)y*(int)PNG->Width) * 4 + 1] /* G */
								* PNG->Image[((int)x + (int)y*(int)PNG->Width) * 4 + 3] / 255;
							B += PNG->Image[((int)x + (int)y*(int)PNG->Width) * 4 + 2] /* B */
								* PNG->Image[((int)x + (int)y*(int)PNG->Width) * 4 + 3] / 255;
							cnt2++;
							weight += (PNG->Image[((int)x + (int)y*(int)PNG->Width) * 4] +   /* R */
								PNG->Image[((int)x + (int)y*(int)PNG->Width) * 4 + 1] +   /* G */
								PNG->Image[((int)x + (int)y*(int)PNG->Width) * 4 + 2]) / 3 /* B */ /* max 255 (RGB pixel's weight) */
								* PNG->Image[((int)x + (int)y*(int)PNG->Width) * 4 + 3] / 255;/* A Transparency (0xAlphaChannel/0xFF) */
						}
					}
					ImgSubWeight[k][l] = weight / cnt2;
					if (l == 0) weightMin = ImgSubWeight[k][l];
					if (ImgSubWeight[k][l] > weightMax) weightMax = ImgSubWeight[k][l];
					if (ImgSubWeight[k][l] < weightMin) weightMin = ImgSubWeight[k][l];
					cnt2 = weight = 0;
				}
			}
			/* Adjusting SUBSECTION contrast-brightness **WARNING** Pixel errors will be amplified too */
			if (weightMax != 0) {
				for (int k = 0; k < CHAR_Y; k++) {
					for (int l = 0; l < CHAR_X; l++) {
						ImgSubWeight[k][l] += 255 - weightMax; //When in a subsec there are only a few 255 white pixels (=weightMax), then ImgSubWeight will be not adjusted
					}
				}
				for (int i = 0; i < CHAR_NUM; i++) {
					for (int k = 0; k < CHAR_Y; k++) {
						for (int l = 0; l < CHAR_X; l++) {
							WeightTemp[i] += abs(ImgSubWeight[k][l] - (CharSet.ARR[i][k][l] - '0') * 255);
						}
					}
					if (i == 0) minWeight = WeightTemp[i];
					if (WeightTemp[i] < minWeight) {
						minWeight = WeightTemp[i];
						minIdx = i;
					}
				}
			}
			PNG->ASCII_Image[pX + pY*PNG->WidthTile] = CharSet.CHAR[minIdx];
			PNG->ASCII_Color[pX + pY*PNG->WidthTile] = TCODColor(R / cnt, G / cnt, B / cnt);
		}
	}
}
/**
 * @brief Calculates CHAR_SET WEIGHT and also SCALING_WEIGHT
 * @param [out] CharSet CHAR_SET type, the weight (brightness) values are stored here
 */
void CalculateWeights(CHAR_SET*CharSet) {
	/* Calculating WEIGHTS... */
	int weight = 0;
	int weight_min = CHAR_X*CHAR_Y;
	int weight_max = 0;
	int k, i, j;
	for (k = 0; k < CHAR_NUM; k++) {
		weight = 0;
		for (i = 0; i < CHAR_Y; i++) {
			for (j = 0; j < CHAR_X; j++) {
				if (CharSet->ARR[k][i][j] == '1') {
					weight++;
				}
			}
		}
		if (weight > weight_max)weight_max = weight;
		if (weight < weight_min)weight_min = weight;
		CharSet->WEIGHT[k] = weight;
	}
	/* Calculating SCALING WEIGHTS... */
	weight = 0;
	for (k = 0; k < CHAR_NUM; k++) {
		/* f(x)=TABLE_MAX*(x-MIN)/(MAX-MIN) => distributing from 0->TABLE_MAX */
		weight = CHAR_X*CHAR_Y*(CharSet->WEIGHT[k] - weight_min) / (weight_max - weight_min);
#ifdef ChangeBlackWhite
		/* if(weight>CHAR_X*CHAR_Y-30) weight=0; */
		if (weight == 0) weight = CHAR_X*CHAR_Y; /* Setting black to white */
#endif /* ChangeBlackWhite */
		CharSet->SCALING_WEIGHT[k] = weight;
	}
}
/**
 * @brief Correspondig CHAR_SETs are calculated for @see CharSetImporter()
 * @param [out] CharSet CHAR_SET type, where the corresponding ASCII CHARs are stored
 * @param [in] char_index Where the iteration was
 * @details if ExtendedASCIICharSet is set, then extended ASCII CHARs are calculated too
 */
void Indexer(CHAR_SET* CharSet, int char_index) {
#ifndef ExtendedASCIICharSet
	if (char_index <= 94) { /* Standard ASCII */
		CharSet->CHAR[char_index] = char_index + 32;
	}
#else
	/* Extended LIBTCOD Terminal ASCII char numbers */
	if (
		(176 <= char_index && 178 >= char_index) || (1 <= char_index && 6 >= char_index) || char_index == 8 ||
		char_index == 219 || char_index == 10 || char_index == 225 || char_index == 14 || char_index == 20 ||
		char_index == 244 || char_index == 13 || char_index == 220 || char_index == 223 || char_index == 228 || char_index == 243
		) {
		//        (0<=char_index&&32>=char_index)||(127<=char_index&&254>=char_index)) { /* Remove specific chars */
		CharSet->CHAR[char_index] = 0;
		for (int i = 0; i < CHAR_Y; i++) {
			for (int j = 0; j < CHAR_X; j++) {
				CharSet->ARR[char_index][i][j] = '0';
			}
		}
	} else
		CharSet->CHAR[char_index] = char_index;
#endif /* ExtendedASCIICharSet */
}

void WriteOutPic(IMAGE* PNG, SIZES TopLeft, SIZES BoxSize) {
	unsigned i, j;
	unsigned VerLeftover = (BoxSize.Y - PNG->HeightTile) / 2; //Függőleges maradék amennyit fent és lent kihagy
	unsigned HorLeftover = (BoxSize.X - PNG->WidthTile) / 2; //Vízszintes maradék amennyit balra és jobbra kihagy
	for (i = 0; i < PNG->HeightTile; i++) {
		for (j = 0; j < PNG->WidthTile; j++) {
			if (PNG->ASCII_Color != NULL) TCODConsole::root->setDefaultForeground(PNG->ASCII_Color[j + i*PNG->WidthTile]);
			if ((TCODConsole::root->getChar(TopLeft.X + HorLeftover + j, TopLeft.Y + VerLeftover + i) == '.' || TCODConsole::root->getChar(TopLeft.X + HorLeftover + j, TopLeft.Y + VerLeftover + i) == ' '))
				TCODConsole::root->putChar(TopLeft.X + HorLeftover + j, TopLeft.Y + VerLeftover + i, PNG->ASCII_Image[j + i*PNG->WidthTile]);
		}
	}
	TCODConsole::root->setDefaultForeground(TCODColor::white);
}
void CalculatePNGSizes(IMAGE* PNG, SUBSECTION* subsec, CONSOLEINFO Con) {
	/* FONTSIZE.Y*subsection_scale*height_tile=height ; FONTSIZE.X*subsection_scale*width_tile=width */
	if ((double)PNG->Height / PNG->Width >= (double)(Con.Size.Y) / Con.Size.X) {
		/* then height_tile is fixed = ConsoleHeight-2 (to fit in console screen) */
		PNG->HeightTile = Con.CharAmount.Y;
		subsec->scale = (double)PNG->Height / (PNG->HeightTile*Con.FontSize.Y);
		PNG->WidthTile = (unsigned int)(PNG->Width / (Con.FontSize.X*subsec->scale));
	} else {
		/* then width_tile is fixed = ConsoleHeight-1 (to fit in console screen) */
		PNG->WidthTile = Con.CharAmount.X;
		subsec->scale = (double)PNG->Width / (PNG->WidthTile*Con.FontSize.X);
		PNG->HeightTile = (unsigned int)(PNG->Height / (Con.FontSize.Y*subsec->scale));
	}

	subsec->height = Con.FontSize.Y*subsec->scale; /* Calculating SUBSECTION in pixels */
	subsec->width = Con.FontSize.X*subsec->scale; /* Calculating SUBSECTION in pixels */

	/*        height_tile = height/14; unsigned subsection_height=14;
	 *        width_tile = width/8;    unsigned subsection_height=8; */
}
