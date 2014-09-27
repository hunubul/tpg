#include "szoba.h"

using namespace std;
void Kiir(szoba asd); //fv-ek prototípusait itt kell feltüntetni (Ez standard programozási követelmény), ezek lehet hogy később átkerülhetnének egy új header-be ha sok lesz belőlük
int main()
{
    srand(time(NULL));
    szoba asd;
    Kiir(asd);
    return 0;
}
void Kiir(szoba asd)
{
    for(int i=0;i<asd.f.size();i++)
    {
        cout<<asd.f[i]<<endl;
    }
}
