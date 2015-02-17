#ifndef HARC_H_INCLUDED
#define HARC_H_INCLUDED

typedef enum {LEFT=4,MID=0,UP=1,RIGHT=2,DOWN=3,NONE=-1} ADIR; //preferált támadási irány
typedef enum {KOR1=0,KOR2=1} ROUND; //éppen ki jön, a player vagy az enemy


class harc {
private:
character p; //a player
character e; //az enemy
ADIR def; //preferált védekezés
ADIR atc; //preferált támadás
ROUND most;


public:
harc(character player, character enemy, std::string prefdef, std::string prefatc);

};













#endif
