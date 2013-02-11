#include "planet.h"
#include "clan.h"
#include "clanmember.h"
#include <iostream>
#include "widgetview.h"

Position Planet::posDepartClan[2] = { Position(HAUTEUR/2,LARGEUR/4) , Position(HAUTEUR/2,3*LARGEUR/4) };

Planet::Planet(QObject *parent) : QThread(parent)
{
    _clan[0] = new Clan(posDepartClan[0],this,JEDI);
    _clan[1] = new Clan(posDepartClan[1],this,SITH);
    init_ressource();
    _clan[0]->init();
    _clan[1]->init();
    _run = true;
}

Planet::~Planet()
{
    delete[] _clan;
    delete [] *_map;
    delete[] _map;
    delete [] *_fightingMap;
    delete [] _fightingMap;
}

void Planet::init_ressource()
{
    Agents *a;

    _map = new Agents**[HAUTEUR];
    *_map = new Agents*[HAUTEUR*LARGEUR];
    _fightingMap= new bool*[HAUTEUR];
    *_fightingMap= new bool[HAUTEUR*LARGEUR];

    for(unsigned i = 1 ; i < HAUTEUR ; ++i)
    {
        _map[i] = _map[i-1] + LARGEUR;
        _fightingMap[i]=_fightingMap[i-1] + LARGEUR;
    }

    for(unsigned i = 0 ; i < HAUTEUR ; ++i)
    {
        for(unsigned j = 0 ; j < LARGEUR ; ++j)
        {
            a = new Agents();

            if( PROBA_RESSOURCE > genrand_int32()%100 )
            {
                unsigned proba = genrand_int32()%100;
                for(unsigned l = 0 ; l < NB_RESSOURSES ; ++l)
                {
                    if(proba < Resource::probaRes[l])
                    {
                        proba = l;
                        break;
                    }
                }
                a->res = new Resource(proba,i,j);
            }
            else
            {
                a->res = NULL;
            }
            a->jedi.nbEclaireur = 0;
            a->jedi.nbGuerrier = 0;
            a->jedi.nbRobot = 0;
            a->sith.nbEclaireur = 0;
            a->sith.nbGuerrier = 0;
            a->sith.nbRobot = 0;
            _map[i][j] = a;
            _fightingMap[i][j]=false;
        }
    }
    for(unsigned i = 0 ; i <NB_PATHFINDER_START ; ++i)
    {
        _clan[0]->addMember(posDepartClan[0],posDepartClan[0],Member_type(1),JEDI);
        _clan[1]->addMember(posDepartClan[1],posDepartClan[1],Member_type(1),SITH);
    }
}

bool ***Planet::getMapClan()
{
    bool ***tab = new bool**[2];
    tab[0] = _clan[0]->getMapVisitee();
    tab[1] = _clan[1]->getMapVisitee();
    return tab;
}

void Planet::run()
{
    _running = true;
    for(_time = 0 ; _running ; )
    {
        QThread::msleep(200);

        if(_run)
        {
            /// Pour voir la map parcouru au total commenter les 2 lignes
            _clan[0]->initMapVisitee();
            _clan[1]->initMapVisitee();
            _clan[0]->execute();
            _clan[1]->execute();

            if(_time%20 == 19)
                naissance();
            emit modelChanged();

            ++_time;
        }
    }
}

void Planet::naissance()
{
    // On génére un nombre entre 0,5 et 1,5.
    double nb = genrand_real1()+0.5;
    unsigned rand;
    nb *= PROBA_NAISS;
    nb = (nb*_clan[0]->getMembers().size())/100.0;
    for(int i = 0 ; i < nb ; ++i)
    {
        if(_clan[0]->getResources().size() > 0)
        {
            rand = genrand_int32()%_clan[0]->getResources().size();
            _clan[0]->addMember(_clan[0]->getResources()[rand]->getPosition(),_clan[0]->getResources()[rand]->getPosition(),Member_type(1),JEDI);
        }
        else
        {
            _clan[0]->addMember(posDepartClan[0],posDepartClan[0],Member_type(1),JEDI);
        }
    }

    nb = genrand_real1()+0.5;
    nb *= PROBA_NAISS;
    nb = (nb*_clan[1]->getMembers().size())/100.0;
    for(int i = 0 ; i < nb ; ++i)
    {
        if(_clan[1]->getResources().size() > 0)
        {
            rand = genrand_int32()%_clan[1]->getResources().size();
            _clan[1]->addMember(_clan[1]->getResources()[rand]->getPosition(),_clan[1]->getResources()[rand]->getPosition(),Member_type(1),SITH);
        }
        else
        {
            _clan[1]->addMember(posDepartClan[1],posDepartClan[1],Member_type(1),SITH);
        }
    }

}

ClanMember* Planet::getMember(Position xy, unsigned clan)
{
    foreach(ClanMember* cm, _clan[clan]->getMembers())
    {
        if(cm->getCurrent() == xy)
            return cm;
    }
    return NULL;
}

bool Planet::allResourceBusy()
{
    std::cout << "Jedi : " << _clan[0]->getResources().size() << "   Sith : " << _clan[1]->getResources().size() << std::endl;
    return Resource::GetNbResource() == _clan[0]->getResources().size() + _clan[1]->getResources().size();
}

Resource* Planet::plusProcheRessourceClan(Position inPos,unsigned inClan)
{
    return _clan[inClan]->plusProcheRessource(inPos);
}
