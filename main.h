/**
 * @file main.h
 * @brief include-ok, beállítások
 */
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

static const int ConsoleWidth=160;
static const int ConsoleHeight=90;

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
