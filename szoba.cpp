#include "szoba.h"
/*ELVILEG MINDEN FELÜLET MEGVAN, DE CSAK A SZEMBE MEG PADLÓ VAN BERAKVA, A BEOLVASANDÓ FÁJLOKAT MÉG BUHERÁLNI KÉNE*/
using namespace std;


/*--------------Konstruktorok kezdete--------------*/
szoba::szoba()
{
    vector<string> fv=padlo();
    int asd=rand() % 4;   //hány tárgy legyen max választva
    for (int i=0; i<asd; i++)   //sorsolás
    {
        int bsd=rand() % fv.size();
        f.push_back(fv[bsd]);
        fv.erase(fv.begin()+bsd);
    }
    vector<string> westv=faltolt();
    asd=1+rand() % 5;   //hány tárgy legyen max választva
    for (int i=0; i<asd; i++)   //sorsolás
    {
        int bsd=rand() % westv.size();
        west.push_back(westv[bsd]);
        westv.erase(westv.begin()+bsd);
    }
    vector<string> eastv=faltolt();
    asd=1+rand() % 5;   //hány tárgy legyen max választva
    for (int i=0; i<asd; i++)   //sorsolás
    {
        int bsd=rand() % eastv.size();
        east.push_back(eastv[bsd]);
        eastv.erase(eastv.begin()+bsd);
    }
    vector<string> northv=faltolt();
    asd=1+rand() % 5;   //hány tárgy legyen max választva
    for (int i=0; i<asd; i++)   //sorsolás
    {
        int bsd=rand() % northv.size();
        north.push_back(northv[bsd]);
        northv.erase(northv.begin()+bsd);
    }
    vector<string> southv=faltolt();
    asd=1+rand() % 5;   //hány tárgy legyen max választva
    for (int i=0; i<asd; i++)   //sorsolás
    {
        int bsd=rand() % southv.size();
        south.push_back(southv[bsd]);
        southv.erase(southv.begin()+bsd);
    }
    vector<string> cv=plafon();
    asd=rand() % 3;   //hány tárgy legyen max választva
    for (int i=0; i<asd; i++)   //sorsolás
    {
        int bsd=rand() % cv.size();
        c.push_back(cv[bsd]);
        cv.erase(cv.begin()+bsd);
    }*/

}

/*--------------Konstruktorok vege--------------*/

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
        throw exception();
        exit(1);
    }
    return floorv;
}
vector<string> szoba::faltolt(){
  ifstream wf ("dolgok/wall.txt");
  vector <string> wv;
  while (wf.good())
  {
    string temp="";
    wf>>ws;
    getline(wf,temp,'\n');
    if (temp!="")
      {
      wv.push_back(temp);
      }
  }
  return wv;
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


/*--------------public fv-ek KEZDETE ------------*/



void szoba::print_f (ostream &out)
/* specifications:
 * inputs :
 *   - vector<string> szoba.f : az adat amit ki akarunk irni
 *   - ostream &out : output stream referencia : a stream ahova ki akarunk irni (lehet cout, vagy fajl is);
 * output :
 *   - ostream &out : -||-
 * format : 1 string / line (no header, no footer)
 */
{
    // fordito direktiva : -std=c++11 // Settings -> Complier... -> Complier Settings -> Complier Flags -> Have g++ follow the C++11 ISO C++ standard
    for(auto s:f)
    {
        out << s << endl;
    }
}

/*--------------public fv-ek VEGE ------------*/
