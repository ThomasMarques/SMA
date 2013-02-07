#ifndef WARRIOR_H
#define WARRIOR_H

#include "clanmember.h"
#include <iostream>

class Warrior : public ClanMember
{
    public:
        Warrior(Position current, Position objectif,Planet * inPlanet,Alliance inAlliance);

        void execute();
        void fight();
};

#endif // WARRIOR_H
