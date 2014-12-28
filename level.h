#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

class level {
private:
    int MidX,MidY;
    int MaxCounter;
    int posX,posY;
    int most; //Irany típusú int
    void merre(IRANY honnan);
    void writearrow();
public:
    std::vector<std::vector<int> > Map;
    std::vector<std::vector<szoba> > terkep; //egyértelmû
    level(int Max_X,int Max_Y,int MidX,int MidY,int MaxCounter);
    void engine();
    void writeout(int posX,int posY);
};
#endif
