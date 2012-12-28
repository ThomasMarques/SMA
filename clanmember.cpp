#include "clanmember.h"
#include "warrior.h"
#include "pathfinder.h"
#include "robot.h"

unsigned ClanMember::_Count = 0;

ClanMember::ClanMember(Position current, Position objectif,Planet* inPlanet,Alliance inAlliance) : _current(current), _objectif(objectif),_planet(inPlanet),_alliance(inAlliance)
{
    _id=++_Count;
    _vector = Position(0,0);
}

ClanMember* ClanMember::Create(Position current, Position objectif, Member_type type,Planet * inPlanet,Alliance inAlliance)
{
    //Factory
    switch(type)
    {
    case Member_type(0) :
        return new Warrior(current,objectif,inPlanet,inAlliance);
    case Member_type(1) :
        if(!inPlanet->allResourceBusy())
            return new Pathfinder(current,objectif,inPlanet,inAlliance);
        else
        {
            if(genrand_int32()%100 < PROBA_NAISS_MERGE)
                return new Pathfinder(current,objectif,inPlanet,inAlliance,true);
            else
                return new Pathfinder(current,objectif,inPlanet,inAlliance);
        }
    case Member_type(2) :
        return new Robot(current,objectif,inPlanet,inAlliance);
    default :
        return NULL;
    }
}

void ClanMember::moveVector()
{
    if(abs(_vector.x) != _cpt.x)
    {
        ++_cpt.x;
        if(_vector.x != 0)
            _current.x+=_vector.x/abs(_vector.x);

        if(_current.x < 0)
            _current.x = _current.x+HAUTEUR;
        else
            _current.x = _current.x%HAUTEUR;
    }
    else
    {
        ++_cpt.y;
        if(_vector.y !=0)
            _current.y+=_vector.y/(abs(_vector.y));

        if(_current.y < 0)
            _current.y = _current.y+LARGEUR;
        else
            _current.y = _current.y%LARGEUR;
        if(_cpt.y == abs(_vector.y)) _cpt=Position(0,0);
    }
    if(_current == _objectif) _vector=Position(0,0);
}

void ClanMember::movePosition()
{
    if(_objectif != _current)
    {
        int x,y;
        x = (abs(HAUTEUR-_objectif.x+_current.x) < abs(_objectif.x-_current.x))?HAUTEUR-_objectif.x+_current.x:_objectif.x-_current.x;
        if(x != 0)
        {
            _current.x += (x < 0)?-1:1;
            if(_current.x < 0)
                _current.x += HAUTEUR;
            else
                _current.x = _current.x%HAUTEUR;
        }
        else
        {
            y = (abs(LARGEUR-_objectif.y+_current.y) < abs(_objectif.y-_current.y))?LARGEUR-_objectif.y+_current.y:_objectif.y-_current.y;

            _current.y += (y < 0)?-1:1;
            if(_current.y < 0)
                _current.y += LARGEUR;
            else
                _current.y = _current.y%LARGEUR;
        }
    }
}

