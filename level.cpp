#include "level.h"

using namespace std;

level::level()
{
vector<vector<int>> fel; //temp vector
  for (int i=0; i<10; i++)
  {
    vector<int> temp;
    for (int j=0; j<10; j++)
    {
      temp.push_back(0); //feltolt 0-val, szoba nélküli térkép
    }
    fel.push_back(temp)
  }
  fel[5][5]=1; //középső innentől szoba
  int a=1; //hanyadik szoba
  while (a<30) //hogy max számig menjen
  {
    for (int i=0; i<10; i++)
    {
      for (int j=0; j<10; j++)
      {
        int b=rand()%5;
        if(fel[i][j]==(0) && fel[i-1][j]!=0 || fel[i+1][j]!=0 || fel[i][j-1]!=0 || fel[i][j+1]!=0 && b==0)
        {
          fel[i][j]=a; //ha van szomszéd szoba, lehet, hogy ez is az lesz
          a++;
        }
      }
    }
  }
  for (int i=0; i<10; i++) //tényleges, követhető térkép csinálása
  {
    for (int j=0; j<10; j++)
    {
      if(fel[i][j]!=0)
      {
        szoba lolol;
        terkep[i][j]=lolol;
      }
    }
  }
}
