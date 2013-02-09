#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>

#include "clanmember.h"
#include "planet.h"

class Robot : public ClanMember
{
private :

    public:
        Robot(Position current, Position objectif,Planet * inPlanet,Alliance inAlliance);

        void execute();
};

#endif // ROBOT_H
