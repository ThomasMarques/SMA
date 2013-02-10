#include "warrior.h"
#include "clan.h"
#include <QVector>

Warrior::Warrior(Position current, Position objectif,Planet * inPlanet,Alliance inAlliance) : ClanMember(current,objectif,inPlanet,inAlliance)
{
    _type=warrior;
    _nbLife=WARRIOR_LIFE;
    _shotValue=WARRIOR_SHOT;
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

    fight();//dans tous les cas, un warrior combat

    if(_alliance == SITH)
     tmpMap[_current.x][_current.y]->sith.nbGuerrier++;
    else
     tmpMap[_current.x][_current.y]->jedi.nbGuerrier++;
}

void Warrior::fight()
{
    getNearestVise(1);//portée de 0
    if(_vise)
    {
        _vise->receiveShot(WARRIOR_SHOT);
    }
}
