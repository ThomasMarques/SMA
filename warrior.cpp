#include "warrior.h"

Warrior::Warrior(Position current, Position objectif,Planet * inPlanet,Alliance inAlliance) : ClanMember(current,objectif,inPlanet,inAlliance)
{
    _type=warrior;
}

void Warrior::execute()
{
    Agents ***tmpMap=_planet->getMap();

    if(_alliance == SITH)
     tmpMap[_current.x][_current.y]->sith.nbGuerrier--;
    else
     tmpMap[_current.x][_current.y]->jedi.nbGuerrier--;

    if(_vector.x == 0 && _vector.y == 0)
        movePosition();
    else
        moveVector();

    if(_alliance == SITH)
     tmpMap[_current.x][_current.y]->sith.nbGuerrier++;
    else
     tmpMap[_current.x][_current.y]->jedi.nbGuerrier++;
}
