#include "clan.h"
#include "commercial.h"
#include <iostream>
#include <QtAlgorithms>

StrategieExploration Clan::_Se;
StrategieAttaque Clan::_Sa;
StrategieDefence Clan::_Sd;

Clan::Clan(Position posDepart, Planet *planete, unsigned alliance)
{
    _argent = 0;
    _planet = planete;
    _posDepart = posDepart;
    _nbWarrior = 0;
    _nbWarriorAdv = 0;
    _alliance = alliance;
    _promotedPhase = true;
    _resChanged = false;

    /// Allocation de la matrice
    _mapConnue = new Agents**[HAUTEUR];
    *_mapConnue = new Agents*[HAUTEUR*LARGEUR];

    for(unsigned i = 1 ; i < HAUTEUR ; ++i)
        _mapConnue[i] = _mapConnue[i-1] + LARGEUR;


    _mapVisitee = new bool*[HAUTEUR];
    *_mapVisitee = new bool[HAUTEUR*LARGEUR];

    for(unsigned i = 1 ; i < HAUTEUR ; ++i)
        _mapVisitee[i] = _mapVisitee[i-1] + LARGEUR;
}

Clan::~Clan()
{
    delete[] _mapConnue;
    delete[] _mapVisitee;
    foreach(ClanMember* c, _members)
        delete c;
    _members.clear();
    foreach(Resource* r, _resources)
        delete r;
    _resources.clear();
}

void Clan::init()
{
    Agents *a;

    /// Initialisation de la matrice
    for(unsigned i = 0 ; i < HAUTEUR ; ++i)
    {
        for(unsigned j = 0 ; j < LARGEUR ; ++j)
        {
            a = new Agents();
            if( (i-_posDepart.x)*(i-_posDepart.x)+(j-_posDepart.y)*(j-_posDepart.y) < PATHFINDER_VIEW*PATHFINDER_VIEW )
            {
                _mapVisitee[i][j] = true;
                a = _planet->getMap()[i][j];
            }
            else
            {
                _mapVisitee[i][j] = false;
                a->res = NULL;
                a->jedi.nbEclaireur = 0;
                a->jedi.nbGuerrier = 0;
                a->jedi.nbRobot = 0;
                a->sith.nbEclaireur = 0;
                a->sith.nbGuerrier = 0;
                a->sith.nbRobot = 0;
                _mapConnue[i][j] = a;
            }
        }
    }
}

void Clan::removeRessource(Resource *resource)
{
    for(QVector<Resource*>::iterator iter = _resources.begin() ; iter != _resources.end() ; ++iter)
    {
        if(*resource == **iter)
        {
            _resChanged = true;
            _resources.erase(iter);
            return;
        }
    }
}

void Clan::removeMember(unsigned inId)
{
    for(QVector<ClanMember*>::iterator iter=_members.begin();iter!=_members.end();++iter)
    {
        if((*iter)->getId() == inId)
        {
            switch((*iter)->getType())
            {
                case pathfinder:
                    if((*iter)->getAlliance() == JEDI)
                        _planet->getMap()[(*iter)->getCurrent().x][(*iter)->getCurrent().y]->jedi.nbEclaireur--;
                    else
                        _planet->getMap()[(*iter)->getCurrent().x][(*iter)->getCurrent().y]->sith.nbEclaireur--;
                break;

                case warrior:
                    if((*iter)->getAlliance() == JEDI)
                        _planet->getMap()[(*iter)->getCurrent().x][(*iter)->getCurrent().y]->jedi.nbGuerrier--;
                    else
                        _planet->getMap()[(*iter)->getCurrent().x][(*iter)->getCurrent().y]->sith.nbGuerrier--;
                break;

                case robot:
                    if((*iter)->getAlliance() == JEDI)
                        _planet->getMap()[(*iter)->getCurrent().x][(*iter)->getCurrent().y]->jedi.nbRobot--;
                    else
                        _planet->getMap()[(*iter)->getCurrent().x][(*iter)->getCurrent().y]->sith.nbRobot--;
                break;
            }
            _members.erase(iter);
            return;
        }
    }
}

void Clan::addMember(Position current, Position objectif,Member_type type,Alliance inAlliance)
{
    switch(type)
    {
        case pathfinder:
            if(inAlliance == JEDI)
                _planet->getMap()[current.x][current.y]->jedi.nbEclaireur++;
            else
                _planet->getMap()[current.x][current.y]->sith.nbEclaireur++;
        break;

        case warrior:
            if(inAlliance == JEDI)
                _planet->getMap()[current.x][current.y]->jedi.nbGuerrier++;
            else
                _planet->getMap()[current.x][current.y]->sith.nbGuerrier++;
        break;

        case robot:
            if(inAlliance == JEDI)
                _planet->getMap()[current.x][current.y]->jedi.nbRobot++;
            else
                _planet->getMap()[current.x][current.y]->sith.nbRobot++;
        break;
    }
    _members.append(ClanMember::Create(current,objectif,type,_planet,inAlliance));
}

void Clan::execute()
{
    int x,y;

    /// MAJ de la strat�gie et des objectifs de chacun
    if(!_planet->allResourceBusy())
        _strategie = &_Se;
    else
    {
        if(_strategie == &_Se)
        {
            // C'est la fin de l'exploration
            foreach(ClanMember *cm, _members)
            {
                cm->setVector(Position(0,0));
            }
        }

        //les clans sont en mode d�fense un temps minium
        if( *(_planet->getT()) < 60)
            _strategie = &_Sd;
        else
            _strategie= &_Sa;
    }

    _strategie->executeStrategie(this);



    /// On appelle la fonction de chaque membre du clan
    foreach(ClanMember *cm, _members)
    {
        cm->execute();
        int vue;

        if(cm->getType() == 0)
            vue = WARRIOR_VIEW;
        else if(cm->getType() == 1)
            vue = PATHFINDER_VIEW;
        else
            vue = ROBOT_VIEW;

        for(int i = cm->getCurrent().x-vue+1 ; i < cm->getCurrent().x+vue ; ++i)
        {
            for(int j = cm->getCurrent().y-vue+1 ; j < cm->getCurrent().y+vue ; ++j)
            {
                x = i-cm->getCurrent().x;
                y = j-cm->getCurrent().y;
                if( x*x+y*y < vue*vue )
                {
                    if(i < 0)
                        x = i + HAUTEUR;
                    else
                        x = i % HAUTEUR;
                    if(j < 0)
                        y = j + LARGEUR;
                    else
                        y = j % LARGEUR;
                    _mapVisitee[x][y] = true;
                    _mapConnue[x][y] = _planet->getMap()[x][y];
                }
            }
        }
    }

    // On compte le nombre de guerrier de chaque clan :
    _nbWarrior = 0;
    _nbWarriorAdv = 0;
    for(unsigned i = 0 ; i < HAUTEUR ; ++i)
    {
        for(unsigned j = 0 ; j < LARGEUR ; ++j)
        {
            if(_alliance == JEDI)
            {
                _nbWarrior += _mapConnue[x][y]->jedi.nbGuerrier;
                _nbWarriorAdv += _mapConnue[x][y]->sith.nbGuerrier;
                _nbPathfinder += _mapConnue[x][y]->jedi.nbEclaireur;
                _nbPathfinderAdv += _mapConnue[x][y]->sith.nbEclaireur;
            }
            else
            {
                _nbWarrior += _mapConnue[x][y]->sith.nbGuerrier;
                _nbWarriorAdv += _mapConnue[x][y]->jedi.nbGuerrier;
                _nbPathfinder += _mapConnue[x][y]->sith.nbEclaireur;
                _nbPathfinderAdv += _mapConnue[x][y]->jedi.nbEclaireur;
            }
        }
    }

    /// On vends toute nos ressources.
    foreach(Resource* res, _resources)
    {
        res->incRessourcesProduite();
        Commercial::getInstance()->sellResources(this,res);
    }

    if(_strategie != &_Se)
    {
        /// On achete autant de robot que possibles.
        Position p;
        do
        {

            p = Position(genrand_int32()%HAUTEUR,genrand_int32()%LARGEUR);
        }
        while(Commercial::getInstance()->achatRobot(this,p));
    }

    std::cout << "Argent du clan : " << _argent << std::endl;
}

void Clan::initMapVisitee()
{
    for(unsigned i = 0 ; i < HAUTEUR ; ++i)
        for(unsigned j = 0 ; j < LARGEUR ; ++j)
            _mapVisitee[i][j] = false;
}

Resource* Clan::plusProcheRessource(Position inPos)
{
    Resource *best = NULL;
    unsigned bestDistance = 9999,dist;
    foreach(Resource *r, _resources)
    {
        dist = abs((abs(LARGEUR-r->getPosition().y+inPos.y) < abs(r->getPosition().y-inPos.y))?LARGEUR-r->getPosition().y+inPos.y:r->getPosition().y-inPos.y) + abs((abs(HAUTEUR-r->getPosition().x+inPos.x) < abs(r->getPosition().x-inPos.x))?HAUTEUR-r->getPosition().x+inPos.x:r->getPosition().x-inPos.x);
        if(dist < bestDistance)
        {
            bestDistance = dist;
            best = r;
        }
    }
    return best;
}

bool lesssThan(const Resource *r1, const Resource *r2 )
{
    //return distanceToMoyenne(r1->getPosition()) > distanceToMoyenne(r2->getPosition());
    return false;
}

void Clan::calculCentreColonie()
{
    /// On recalcul le centre uniquement si on a supprim� ou ajout� des ressources.
    if(_resChanged)
    {
        float x = 0;
        float y = 0;
        unsigned count = 0;
        foreach(Resource *res, _resources)
        {
            x += res->getPosition().x;
            y += res->getPosition().y;

            ++count;
        }
        _moyenne = Position(x/count,y/count);

        qSort ( _resources.begin(), _resources.end(), lesssThan );
    }
}



