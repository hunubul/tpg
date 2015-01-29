#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

typedef struct {
    std::string effect; //blind bleed poison satöbbi
    int roundsleft; //mennyi körig van még fent az effect
} balfasz;

class character{
private:
  int maxhp; //egyértelmű
  int hp; //jelenlegi hp
  int maxstam; //maximum stamina
  int stam; //jelenlegi stamina
  int defense; //védelmi érték
  int offense; //támadó érték
  std::vector<balfasz> status; //blind bleed etc ide, struktúra feljebb
  std::vector<ITEMS> inventory; //táska tartalma
  std::vector<ITEMS> wearing; //amit hord, ez külön kezelendő inventorytól
public:
  character(); //konstruktor, jelenleg üres
};

#endif
