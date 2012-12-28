#include "pathfinder.h"
#include "clan.h"
#include <math.h>
#include <cfloat>
#include <cstdlib>
#include <iostream>

Pathfinder::Pathfinder(Position current, Position objectif,Planet * inPlanet,Alliance inAlliance) : ClanMember(current,objectif,inPlanet,inAlliance)
{
    _vector=Position(0,0);
    _cpt=Position(0,0);
    _promoted=false;
    _trouve = false;
    _gonnaMerge=false;
    _targetToMerged=NULL;
    _type=pathfinder;
}

Pathfinder::Pathfinder(Position current, Position objectif,Planet* inPlanet,Alliance inAlliance,bool inPromote) : ClanMember(current,objectif,inPlanet,inAlliance)
{
    _vector=Position(0,0);
    _cpt=Position(0,0);
    _promoted=inPromote;
    _trouve = false;
    _gonnaMerge=false;
    _targetToMerged=NULL;
    _type=pathfinder;
    cout <<"Crea Pathfinder merge"<<endl;
}

void Pathfinder::execute()
{
    Agents ***tmpMap=_planet->getMap();

    if(_alliance == SITH)
        tmpMap[_current.x][_current.y]->sith.nbEclaireur--;
    else
        tmpMap[_current.x][_current.y]->jedi.nbEclaireur--;

    if(!_promoted && _vector.x == 0 && _vector.y==0)
    {
        //WAIT
    }
    else
    {
        if(!_promoted)
        {
            int x,y;
            if(_trouve && _planet->getMap()[_objectif.x][_objectif.y]->res->getClan() == NULL)
            {
                if(_objectif.x == _current.x && _objectif.y == _current.y)
                {
                    _planet->getMap()[_objectif.x][_objectif.y]->res->setClan(_planet->getClan(_alliance));
                    _planet->getClan(_alliance)->addRessource(_planet->getMap()[_objectif.x][_objectif.y]->res);
                }
                else
                {
                    movePosition();

                    if(_alliance == SITH)
                        tmpMap[_current.x][_current.y]->sith.nbEclaireur++;
                    else
                        tmpMap[_current.x][_current.y]->jedi.nbEclaireur++;
                    return;
                }
            }

            _trouve = false;

            if(!_trouve)
            {
                unsigned best = 9999,tmp;
                for(int i = _current.x-PATHFINDER_VIEW+1 ; i < _current.x+PATHFINDER_VIEW ; ++i)
                {
                    for(int j = _current.y-PATHFINDER_VIEW+1 ; j < _current.y+PATHFINDER_VIEW ; ++j)
                    {
                        x = i-_current.x;
                        y = j-_current.y;
                        tmp = x*x+y*y;
                        if( tmp < PATHFINDER_VIEW*PATHFINDER_VIEW )
                        {
                            if(i < 0)
                                x = i + HAUTEUR;
                            else
                                x = i % HAUTEUR;
                            if(j < 0)
                                y = j + LARGEUR;
                            else
                                y = j % LARGEUR;
                            if(_planet->getMap()[x][y]->res != NULL)
                            {
                                bool vise = (_alliance == JEDI)?_planet->getMap()[x][y]->res->_viseJedi:_planet->getMap()[x][y]->res->_viseSith;
                                if(_planet->getMap()[x][y]->res->getClan() == NULL && !vise)
                                {
                                    if(_trouve)
                                    {
                                        if(tmp < best)
                                        {
                                            if(_alliance == JEDI)
                                            {
                                                _planet->getMap()[_objectif.x][_objectif.y]->res->_viseJedi = false;
                                                _planet->getMap()[x][y]->res->_viseJedi = true;
                                            }
                                            else
                                            {
                                                _planet->getMap()[_objectif.x][_objectif.y]->res->_viseSith = false;
                                                _planet->getMap()[x][y]->res->_viseSith = true;
                                            }
                                            _objectif = Position(x,y);
                                            best = tmp;
                                        }
                                    }
                                    else
                                    {
                                       _objectif = Position(x,y);
                                        best = tmp;
                                        if(_alliance == JEDI)
                                        {
                                            _planet->getMap()[x][y]->res->_viseJedi = true;
                                        }
                                        else
                                        {
                                            _planet->getMap()[x][y]->res->_viseSith = true;
                                        }
                                    }
                                    _trouve = true;
                                }
                            }
                        }
                    }
                }
            }
            if(_trouve)
            {
                if(_objectif.x == _current.x && _objectif.y == _current.y)
                {
                    _planet->getMap()[_objectif.x][_objectif.y]->res->setClan(_planet->getClan(_alliance));
                    _planet->getClan(_alliance)->addRessource(_planet->getMap()[_objectif.x][_objectif.y]->res);
                    _trouve = false;
                }
                else
                {
                    movePosition();
                }
            }
            else
            {
                moveVector();
            }
        }
        else
        {
            if(!_gonnaMerge)
            {
                //calcul d'un nouveau vecteur
                //déplacement vers le pathfinder le plus proche
                Pathfinder* pth,*best=NULL;
                float tmpDist=FLT_MAX,bestDist=FLT_MAX,delta_x,delta_y;
                cout <<"Exec merge"<<endl;
                foreach(ClanMember* m, _planet->getClan(_alliance)->getMembers())
                {
                    if(m->getType() == pathfinder)
                    {
                        pth=(Pathfinder*)m;
                        if(pth->getId()!= _id && !pth->_gonnaMerge)
                        {
                            delta_x=(pth->getCurrent().x-_current.x)*(pth->getCurrent().x-_current.x);
                            delta_y=(pth->getCurrent().y-_current.y)*(pth->getCurrent().y-_current.y);
                            tmpDist=sqrt(delta_x+delta_y);
                            if(tmpDist < bestDist)
                            {
                                bestDist=tmpDist;
                                best=pth;
                            }

                        }
                    }
                }
                if(best != NULL) //si un pathfinder promu trouvé
                {
                    _objectif=Position((_current.x+best->getCurrent().x)/2,(_current.y+best->getCurrent().y)/2);
                    _vector.x=_objectif.x-_current.x;
                    _vector.y=_objectif.y-_current.y;

                    _targetToMerged=best;
                    _cpt=Position(0,0);
                    ++_cpt.x;
                    if(_vector.x != 0)
                        _current.x+=_vector.x/abs(_vector.x);
                    else
                         _current.x+=_vector.x/1;
                    if(_current.x < 0)
                        _current.x = _current.x+HAUTEUR;
                    else
                        _current.x = _current.x%HAUTEUR;
                    _gonnaMerge=true;

                    //MAJ best
                    best->_objectif=_objectif;
                    best->_vector.x=_objectif.x-best->_current.x;
                    best->_vector.y=_objectif.y-best->_current.y;
                    best->_cpt=Position(0,0);
                    best->_gonnaMerge=true;
                    best->_promoted=true;
                    best->_targetToMerged=this;
                }
            }
            else//simplement avancer
            {
                if(_current != _objectif)//si objectif pas atteint
                    moveVector();
                else
                    _vector=Position(0,0);
            }
        }
    }

    if(_alliance == SITH)
        tmpMap[_current.x][_current.y]->sith.nbEclaireur++;
    else
        tmpMap[_current.x][_current.y]->jedi.nbEclaireur++;
}
