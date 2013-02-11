#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QVector>
#include <iostream>
#include "clanmember.h"

class Warrior;

class Pathfinder : public ClanMember
{
    private:
        bool _trouve;
        bool _promoted;
        bool _gonnaMerge;
        Pathfinder* _targetToMerged;

        Resource * _resourceTargeted;
        unsigned _currentCatching;

        QList<Warrior*> _followers;


    public:
        Pathfinder(Position current, Position objectif,Planet* inPlanet,Alliance inAlliance);
        Pathfinder(Position current, Position objectif,Planet* inPlanet,Alliance inAlliance,bool inPromote);
        bool isPromoted() { return _promoted; }
        bool isGoingToMerge() { return _gonnaMerge; }
        void setPromoted(bool inProm) { _promoted=inProm; }
        void setGonnaMerge(bool inGm) {_gonnaMerge=inGm;}
        Pathfinder* getTargetToMerged() {return _targetToMerged;}
        Resource * getResourceTargeted(){ return _resourceTargeted;}
        QList<Warrior*> getFollowers() { return _followers;}
        void setResourceTargeted(Resource * inRes) { _resourceTargeted=inRes;}
        bool catchingRessource();
        void addFollower(Warrior* inW);
        void removeFollower(Warrior * inW);
        void removeFollowers();
        void execute();
};

#endif // PATHFINDER_H
