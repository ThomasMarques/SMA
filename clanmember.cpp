#include "clanmember.h"
#include "warrior.h"
#include "pathfinder.h"
#include "robot.h"
#include "clan.h"

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

void ClanMember::receiveShot(int inShotValue)
{
    _nbLife-=inShotValue;
    _planet->getFightingMap()[_current.x][_current.y]=true;
    if(_nbLife <= 0) //destruction du ClanMember
    {
        foreach(ClanMember* members, _shootersList)
            members->setVise(NULL);

        _planet->getClan(_alliance)->removeMember(_id);
    }
}

void ClanMember::getNearestVise(int view)
{
    bool finded = false;

    if(!finded)
    {
        /// On cherche l'ennemie
        unsigned x,y,tmp;

        for(int i = _current.x - view; i < view + _current.x && !finded ; ++i)
        {
            for(int j = _current.y - view; j < view + _current.y && !finded ; ++j)
            {
                x = i-_current.x;
                y = j-_current.y;
                tmp = x*x+y*y;
                if( tmp < view*view )
                {
                    if(i < 0)
                        x = i + HAUTEUR;
                    else
                        x = i % HAUTEUR;
                    if(j < 0)
                        y = j + LARGEUR;
                    else
                        y = j % LARGEUR;

                    unsigned adv;
                    if(_alliance == JEDI)
                        adv = _planet->getMap()[x][y]->sith.nbEclaireur +
                                _planet->getMap()[x][y]->sith.nbGuerrier +
                                _planet->getMap()[x][y]->sith.nbGuerrier;
                    else
                        adv = _planet->getMap()[x][y]->jedi.nbEclaireur +
                                _planet->getMap()[x][y]->jedi.nbGuerrier +
                                _planet->getMap()[x][y]->jedi.nbGuerrier;

                    if(adv > 0)
                    {
                        if(_alliance == JEDI)
                        {
                            _vise = _planet->getMember(Position(x,y),1);
                        }
                        else
                        {
                            _vise = _planet->getMember(Position(x,y),0);
                        }
                        if(_vise != NULL)
                        {
                            finded = true;
                            _vise->getShootersList().push_back(this);
                        }
                    }
                }
            }
        }
    }

    if(!finded)
    {
        _vise = NULL;
    }
}

