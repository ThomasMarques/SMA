#include "robot.h"

Robot::Robot(Position current, Position objectif,Planet * inPlanet,Alliance inAlliance) : ClanMember(current,objectif,inPlanet,inAlliance)      
{
    _type=robot;
}




