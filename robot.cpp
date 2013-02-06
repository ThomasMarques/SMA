#include "robot.h"
#include "clan.h"
#include <iostream>

Robot::Robot(Position current, Position objectif,Planet * inPlanet,Alliance inAlliance) : ClanMember(current,objectif,inPlanet,inAlliance)      
{
    _type=robot;
    _vise = NULL;
    _nbLife=ROBOT_LIFE;
    _shotValue=ROBOT_SHOT;
}


void Robot::execute()
{
    getNearestVise(ROBOT_SHOT);
    if(_vise != NULL)
    {
        /// Le tir est déporté pour le moment.
    }
}



