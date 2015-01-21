/**
 * @file main.cpp
 * @brief main függvény
 */
#include "main.h"

using namespace std;
int ConsoleWidth,ConsoleHeight;
int FontX=8;
int FontY=8;

int main() {
    srand(time(NULL));
    TCODSystem::getCurrentResolution(&ConsoleWidth,&ConsoleHeight);
    ConsoleWidth /= FontX;
    ConsoleHeight /= FontY;
    TCODConsole::initRoot(ConsoleWidth,ConsoleHeight,"TPG");
    #ifndef DEBUG
    TCODConsole::setFullscreen(true);
    #endif // DEBUG
    level asd(17,17,8,8,80); /* A pálya inicializálása */
    chest Chest; /* Chest inicializálás */
    asd.engine(); /* A fõ loop */
    return 0;
}
