#ifndef HARC_H_INCLUDED
#define HARC_H_INCLUDED

typedef enum {LEFT=4,MID=0,UP=1,RIGHT=2,DOWN=3,NONE=-1} ADIR;


class harc {
private:

public:
harc(character player, character enemy, std::string prefdef, std::string prefatc);
};













#endif
