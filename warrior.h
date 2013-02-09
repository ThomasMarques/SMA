#ifndef WARRIOR_H
#define WARRIOR_H

#include <iostream>
#include "clanmember.h"


class Warrior : public ClanMember
{
    public:
        Warrior(Position current, Position objectif,Planet * inPlanet,Alliance inAlliance);

        void execute();
        void fight();
};

#endif // WARRIOR_H
