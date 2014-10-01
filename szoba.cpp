#include "szoba.h"
/*ELVILEG MINDEN FELÜLET MEGVAN, A BEOLVASANDÓ FÁJLOKAT MÉG BUHERÁLNI KÉNE*/
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
    vector<string> w2v=fal2();
    asd=1+rand() % 3;   //hány tárgy legyen max választva
    for (int i=0; i<asd; i++)   //sorsolás
    {
        int bsd=rand() % w2v.size();
        w2.push_back(w2v[bsd]);
        w2v.erase(w2v.begin()+bsd);
    }
    vector<string> w3v=fal3();
    asd=rand() % 3;   //hány tárgy legyen max választva
    for (int i=0; i<asd; i++)   //sorsolás
    {
        int bsd=rand() % w3v.size();
        w3.push_back(w3v[bsd]);
        w3v.erase(w3v.begin()+bsd);
    }
    vector<string> w4v=fal4();
    asd=rand() % 3;   //hány tárgy legyen max választva
    for (int i=0; i<asd; i++)   //sorsolás
    {
        int bsd=rand() % w4v.size();
        w4.push_back(w4v[bsd]);
        w4v.erase(w4v.begin()+bsd);
    }
    vector<string> cv=plafon();
    asd=rand() % 3;   //hány tárgy legyen max választva
    for (int i=0; i<asd; i++)   //sorsolás
    {
        int bsd=rand() % cv.size();
        c.push_back(cv[bsd]);
        cv.erase(cv.begin()+bsd);
    }

}

/*--------------beolvas fgv-ek--------------*/
vector<string> szoba::padlo()
{
    ifstream floorf ("dolgok/f.txt");
    vector <string> floorv;
    if (!floorf.fail()) //Ezt teljesen fölösleges belerakni, nem dolgozunk nem létező fájlokkal ám...
    //A többit enélkül megcsinálom, ettől nagyon hosszú lesz a kód, de amíg prealfában van akár belerakhatod
    {
        while (floorf.good())
        {
            string temp="";
            floorf>>ws; //Ez kell??? Jah, főleg az utolsónál, hogy ne legyen egy enter a vektor utolsó tagjaként.
            //Simán be tudja olvasni, és ártani nem árt
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
vector<string> fal2(){
  ifstream w2f ("dolgok/w2.txt");
  vector <string> w2v;
  while (w2f.good())
  {
    string temp="";
    w2f>>ws;
    getline(w2f,temp,'\n');
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
    getline(w3f,temp,'\n');
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
    getline(w4f,temp,'\n');
    if (temp!="")
      {
      w4v.push_back(temp);
      }
  }
  return w4v;
}
vector<string> plafon(){
  ifstream cf ("dolgok/c.txt");
  vector <string> c;
  while (cf.good())
  {
    string temp="";
    cf>>ws;
    getline(cf,temp,'\n');
    if (temp!="")
      {
      c.push_back(temp);
      }
  }
  return c;
}
/*--------------beolvas fgv-ek vege--------------*/
