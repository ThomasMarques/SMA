#ifndef STRUCTURE_H
#define STRUCTURE_H

struct Position {
    int x;
    int y;
    Position(){x=0;y=0;}
    Position(int inX,int inY) : x(inX), y(inY) {}
    bool operator==(const Position& inPos) {return x==inPos.x && y==inPos.y;}
    bool operator!=(const Position& inPos) {return !(*this==inPos);}
};


struct cptIndividu{
    unsigned nbGuerrier;
    unsigned nbEclaireur;
    unsigned nbRobot;
};

class Resource;

struct Agents{
    cptIndividu jedi;
    cptIndividu sith;
    Resource *res;
};

#endif // STRUCTURE_H
