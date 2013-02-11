#ifndef WARRIOR_H
#define WARRIOR_H

#include <iostream>
#include "clanmember.h"
#include "pathfinder.h"


class Warrior : public ClanMember
{
    private:
    Pathfinder * _following;

    public:
        Warrior(Position current, Position objectif,Planet * inPlanet,Alliance inAlliance);
        Pathfinder* getFollowing() {return _following;}
        void setFollowing(Pathfinder * inP){_following=inP;}

        void execute();
        void fight();
};

#endif // WARRIOR_H
