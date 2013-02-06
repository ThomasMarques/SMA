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

    if(_alliance == SITH)
     tmpMap[_current.x][_current.y]->sith.nbGuerrier++;
    else
     tmpMap[_current.x][_current.y]->jedi.nbGuerrier++;
}

void Warrior::fightCurrent()
{
   /* cptIndividu currentAg;
    int alli;
    if(_alliance == JEDI)
    {
        currentAg=_planet->getMap()[_current.x][_current.y]->sith;
        alli=SITH;
    }
    else
    {
        currentAg=_planet->getMap()[_current.x][_current.y]->jedi;
        alli=JEDI;
    }

    if(currentAg.nbEclaireur > 0) // alors on les combat (100% de victoire)
    {
        QVector<ClanMember*> tmp= _planet->getClan(alli).getMembers();
        QVector<ClanMember*>::iterator ite;

        _planet->getFightingMap()[_current.x][_current.y]=true;
    }*/
}
