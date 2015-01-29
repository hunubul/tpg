#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

class character{
private:
  int maxhp; //egyértelmű
  int hp; //jelenlegi hp
  int maxstam; //maximum stamina
  int stam; //jelenlegi stamina
  int defense; //védelmi érték
  int offense; //támadó érték
  vector<ITEMS> inventory; /táska tartalma
  vector<ITEMS> wearing; //amit hord, ez külön kezelendő inventorytól
public:
  character(); //konstruktor, jelenleg üres
};

#endif
