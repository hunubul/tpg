#include "szoba.h"

using namespace std;
//fv-ek prototípusait itt kell feltüntetni (Ez standard programozási követelmény), ezek lehet hogy később átkerülhetnének egy új header-be ha sok lesz belőlük
//void Kiir(szoba asd); //TAGFUGGVENY

int main()
{
    srand(time(NULL));
    szoba asd;
    asd.print_f(cout);
    return 0;
}
