#ifndef HARC_H_INCLUDED
#define HARC_H_INCLUDED

#include "libtcod.hpp"
#include "character.h"

typedef enum {PLAYER_KOR=0,ENEMY_KOR=1} ROUND; //éppen ki jön, a player vagy az enemy

typedef struct {
    std::string msg;
    TCOD_color_t color;
} CONLOG;

void PlayerAttack(enemy &e,int selIndx,std::vector<CONLOG> &con_log);
void EnemyAttack(enemy &e,std::vector<CONLOG> &con_log);
void PrintPlayerStats();
void MenuSelection(int StartingPos,int selIndx,std::string choices[],TCOD_key_t&input,std::vector<CONLOG> &con_log);
void HarcGUI(enemy e,ROUND most=(ROUND)0);

#endif
