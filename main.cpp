/**
 * @file main.cpp
 * @brief main függvény
 */
#include "main.h"

using namespace std;
int ConsoleWidth,ConsoleHeight;
int FontX=8;
int FontY=8;
SIZES UpperBoxSiz, UpperBoxPos;
SIZES SideBoxSiz,  SideBoxPosLeft, SideBoxPosRight;
SIZES BottomBoxSiz,BottomBoxPos;
SIZES MiddleBoxSiz,MiddleBoxPos;

//4:3  - 1024x768
//16:9 - 1280x720

int main() {
    srand(time(NULL));
    TCODSystem::getCurrentResolution(&ConsoleWidth,&ConsoleHeight);
//    ConsoleWidth = 1024;
//    ConsoleHeight = 768;
    ConsoleWidth /= FontX;
    ConsoleHeight /= FontY;
    //UpperBoxSiz  = (SIZES){ConsoleWidth*634/1280,ConsoleHeight*174/720};
    UpperBoxSiz  = (SIZES){ConsoleWidth,ConsoleHeight*174/720};
    //SideBoxSiz   = (SIZES){ConsoleWidth*316/1280,ConsoleHeight*355/720};
    SideBoxSiz   = (SIZES){ConsoleWidth*316/1280,ConsoleHeight};
    //BottomBoxSiz = (SIZES){ConsoleWidth*634/1280,ConsoleHeight*180/720};
    BottomBoxSiz = (SIZES){ConsoleWidth,ConsoleHeight*180/720};
    MiddleBoxSiz = (SIZES){ConsoleWidth*628/1280,ConsoleHeight*354/720};
    UpperBoxPos  = (SIZES){(ConsoleWidth-UpperBoxSiz.X)/2,0};
    SideBoxPosLeft   = (SIZES){0,(ConsoleHeight-SideBoxSiz.Y)/2};
    SideBoxPosRight  = (SIZES){ConsoleWidth-SideBoxSiz.X,(ConsoleHeight-SideBoxSiz.Y)/2};
    BottomBoxPos = (SIZES){(ConsoleWidth-BottomBoxSiz.X)/2,ConsoleHeight-BottomBoxSiz.Y};
    MiddleBoxPos = (SIZES){(ConsoleWidth-MiddleBoxSiz.X)/2,(ConsoleHeight-MiddleBoxSiz.Y)/2};
    TCODConsole::initRoot(ConsoleWidth,ConsoleHeight,"TPG");
#ifndef DEBUG
    TCODConsole::setFullscreen(true);
#endif // DEBUG
    level asd(17,17,8,8,80); /* A pálya inicializálása */
    chest Chest; /* Chest inicializálás */
    asd.engine(); /* A fõ loop */
    return 0;
}
