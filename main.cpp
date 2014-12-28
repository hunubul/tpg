/**
 * @file main.cpp
 * @brief main függvény
 */
#include "main.h"

using namespace std;

int main() {
    srand(time(NULL));
    TCODConsole::initRoot(ConsoleWidth,ConsoleHeight,"TPG");
    level asd(17,17,8,8,80); /* A pálya inicializálása */
    chest Chest; /* Chest inicializálás */
    asd.engine(); /* A fõ loop */
    return 0;
}
