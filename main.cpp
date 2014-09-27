#include "szoba.cpp"

using namespace std;
void Kiir(szoba asd)
{
    for(int i=0;i<asd.f.size();i++)
    {
        cout<<asd.f[i]<<endl;
    }
}
int main()
{
    srand(time(NULL));
    szoba asd;
    Kiir(asd);
    return 0;
}
