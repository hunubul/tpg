#include "szoba.h"
/*MÉG CSAK PADLÓ VAN IMPLEMENTÁLVA, MINDEN MÁS FELÜLET KI VAN KOMMENTEZVE, ELVILEG ELÉG KISZEDNI, HOGY MÛKÖDJÖN*/
using namespace std;

szoba::szoba()
{
    vector<string> fv=padlo();
    int asd=rand() % 5;   //hány tárgy legyen max választva
    for (int i=0; i<asd; i++)   //sorsolás
    {
        int bsd=rand() % fv.size();
        f.push_back(fv[bsd]);
        fv.erase(fv.begin()+bsd);
    }


    /*vector<string> w2v=fal2();
      vector<string> w3v=fal3();
      vector<string> w4v=fal4();
      vector<string> cv=plafon();*/

}

/*--------------beolvas fgv-ek--------------*/
vector<string> szoba::padlo()
{
    ifstream floorf ("dolgok/f.txt");
    vector <string> floorv;
    if (!floorf.fail())
    {
        while (floorf.good())
        {
            string temp="";
            //floorf>>ws; //Ez kell???
            getline(floorf,temp,'\n'); //mindenhova getline kell valszleg
            if (temp!="")
            {
                floorv.push_back(temp);
            }
        }
    }
    else
    {
        cout << "Error: couldn't find \"dolgok/f.txt\"\n";
        exit(1);
    }
    return floorv;
}
/*
vector<string> fal2(){
  ifstream w2f ("dolgok/w2.txt");
  vector <string> w2v;
  while (w2f.good())
  {
    string temp="";
    w2f>>ws;
    w2f>>temp;
    if (temp!="")
      {
      w2v.push_back(temp);
      }
  }
  return w2v;
}
vector<string> fal3(){
  ifstream w2f ("dolgok/w3.txt");
  vector <string> w3v;
  while (w3f.good())
  {
    string temp="";
    w3f>>ws;
    w3f>>temp;
    if (temp!="")
      {
      w3v.push_back(temp);
      }
  }
  return w3v;
}
vector<string> fal4(){
  ifstream w2f ("dolgok/w4.txt");
  vector <string> w4v;
  while (w4f.good())
  {
    string temp="";
    w4f>>ws;
    w4f>>temp;
    if (temp!="")
      {
      w4v.push_back(temp);
      }
  }
  return w4v;
}
vector<string> plafon(){
  ifstream c ("dolgok/c.txt");
  vector <string> c;
  while (c.good())
  {
    string temp="";
    c>>ws;
    c>>temp;
    if (temp!="")
      {
      c.push_back(temp);
      }
  }
  return c;
}*/
/*--------------beolvas fgv-ek vege--------------*/