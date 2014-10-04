#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED
#include "szoba.h"
struct koor
{
  public:
  int x,y;
};
class level
{
  public:
  std::vector<vector<koor>> terkep; //egyértelmű
  level();
};
#endif
