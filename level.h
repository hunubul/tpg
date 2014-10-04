#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED
#include "szoba.h"
class level
{
  public:
  std::vector<vector<szoba::szoba>> terkep; //egyértelmű
  int ii; //jelenlegi i koordináta
  int jj; //jelenlegi j koordináta
  level();
}
#endif
