#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "clanmember.h"
#include <QVector>
#include <iostream>

class Pathfinder : public ClanMember
{
    private:
        bool _trouve;
        bool _promoted;
        bool _gonnaMerge;
        Pathfinder* _targetToMerged;

    public:
        Pathfinder(Position current, Position objectif,Planet* inPlanet,Alliance inAlliance);
        bool isPromoted() { return _promoted; }
        void setPromoted(bool inProm) { _promoted=inProm; }
        void setGonnaMerge(bool inGm) {_gonnaMerge=inGm;}
        Pathfinder* getTargetToMerged() {return _targetToMerged;}
        void execute();
};

#endif // PATHFINDER_H
