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
    std::vector<std::vector<int> > FOV;
    std::vector<std::vector<szoba> > terkep; //egyértelmû
    level(int Max_X,int Max_Y,int MidX,int MidY,int MaxCounter);
    void engine();
    void writeout(int posX,int posY);

    void RoomWriteout();
    void WriteOutBoxes();
    void Pic2ASCII(std::string PicName,SIZES TopLeft,SIZES BoxSize);
    void WriteOutPic(IMAGE PNG,SIZES TopLeft,SIZES BoxSize);
    void CalculatePNGSizes(IMAGE* PNG,SUBSECTION* subsec,CONSOLEINFO Con);
};
#endif
