/**
 * @file main.h
 * @brief include-ok, beállítások
 */
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

/* Define this to enable DEBUG MODE */
#define DEBUG
#define MAX_RARITY 10

#include "Image2ASCIIcolors.h"

extern int ConsoleWidth;
extern int ConsoleHeight;
extern int FontX;
extern int FontY;
extern SIZES UpperBoxSiz, UpperBoxPos;
extern SIZES SideBoxSiz,  SideBoxPosLeft, SideBoxPosRight;
extern SIZES BottomBoxSiz,BottomBoxPos;
extern SIZES MiddleBoxSiz,MiddleBoxPos;
/** @brief enum az iránynak */
typedef enum {LEFT=3,UP=0,RIGHT=1,DOWN=2} IRANY;

#include "libtcod.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "szoba.h"
#include "itemek.h"
#include "level.h"


#endif // MAIN_H_INCLUDED
