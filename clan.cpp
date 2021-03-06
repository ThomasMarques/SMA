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
    _nbRobot = 0;

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
        ++_nbRobot;
        break;
    }
    _members.append(ClanMember::Create(current,objectif,type,_planet,inAlliance));
}

float redefDistance(const Position &pos1, const Position &pos2)
{
    float normal = sqrt((pos1.x-pos2.x)*(pos1.x-pos2.x)+(pos1.y-pos2.y)*(pos1.y-pos2.y));
    float gauche = sqrt((pos1.x-(pos2.x-LARGEUR))*(pos1.x-(pos2.x-LARGEUR))+(pos1.y-pos2.y)*(pos1.y-pos2.y));
    float droite = sqrt((pos1.x-(pos2.x+LARGEUR))*(pos1.x-(pos2.x+LARGEUR))+(pos1.y-pos2.y)*(pos1.y-pos2.y));
    float haut = sqrt((pos1.x-pos2.x)*(pos1.x-pos2.x)+(pos1.y-(pos2.y-HAUTEUR))*(pos1.y-(pos2.y-HAUTEUR)));
    float bas = sqrt((pos1.x-pos2.x)*(pos1.x-pos2.x)+(pos1.y-(pos2.y+HAUTEUR))*(pos1.y-(pos2.y+HAUTEUR)));

    float ret = (bas < haut)?bas:haut;
    ret = (ret < droite)?ret:droite;
    ret = (ret < gauche)?ret:gauche;
    ret = (ret < normal)?ret:normal;

    return ret;
}

bool lessThan( const Resource* e1, const Resource* e2 )
{
    return redefDistance(Resource::getCompare(),e1->getPosition()) < redefDistance(Resource::getCompare(),e2->getPosition());
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
            getCentreColonie();
            int i = 0;
        }

        //les clans sont en mode d�fense un temps minium
        if(_members.count() <= _planet->getClan((_alliance+1)%2)->getMembers().count())
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
                //_nbWarrior += _mapConnue[x][y]->jedi.nbGuerrier;
                _nbWarrior+=_planet->getMap()[x][y]->jedi.nbGuerrier;
                _nbWarriorAdv += _mapConnue[x][y]->sith.nbGuerrier;
                _nbPathfinder += _mapConnue[x][y]->jedi.nbEclaireur;
                _nbPathfinderAdv += _mapConnue[x][y]->sith.nbEclaireur;
            }
            else
            {
                _nbWarrior+=_planet->getMap()[x][y]->sith.nbGuerrier;
               // _nbWarrior += _mapConnue[x][y]->sith.nbGuerrier;
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

        Position centreOpponent = _planet->getCentreColonie(!_alliance);
        Resource::setCompare(centreOpponent);
        qSort(_resources.begin(), _resources.end(), lessThan);

        Position p;
        do
        {
            Resource *res =_resources.at(_nbRobot%_resources.size());
            p = Position(res->getPosition().x,res->getPosition().y);

        } while(Commercial::getInstance()->achatRobot(this,p));
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

Position Clan::getCentreColonie()
{
    /// On recalcul le centre uniquement si on a supprim� ou ajout� des ressources.
    if(_resChanged)
    {
        unsigned debutX = 0;
        unsigned finX = LARGEUR - 1;
        unsigned debutY = 0;
        unsigned finY = HAUTEUR - 1;
        Position temp;
        Position best;
        float bestDist;
        float dist;
        int intervalX;
        int intervalY;


        while(debutX != finX && debutY != finY)
        {
            std::cout << debutX << " " << finX << " - " << debutY << " " << finY << std::endl;
            dist = 0;
            bestDist = 0;
            intervalX = finX-debutX;
            intervalY = finY-debutY;

            best = Position((intervalX)/4 + debutX, (intervalY)/4 + debutY);
            foreach(Resource *res, _resources)
            {
                bestDist += distance(best,res->getPosition());
            }

            dist = 0;

            temp = Position((intervalX*3)/4 + debutX, (intervalY)/4 + debutY);
            foreach(Resource *res, _resources)
            {
                dist += distance(temp,res->getPosition());
            }

            if(dist < bestDist)
            {
                bestDist = dist;
                best = temp;
            }

            dist = 0;

            temp = Position((intervalX)/4 + debutX, (intervalY*3)/4 + debutY);
            foreach(Resource *res, _resources)
            {
                dist += distance(temp,res->getPosition());
            }

            if(dist < bestDist)
            {
                bestDist = dist;
                best = temp;
            }

            dist = 0;

            temp = Position((intervalX*3)/4 + debutX, (intervalY*3)/4 + debutY);
            foreach(Resource *res, _resources)
            {
                dist += distance(temp,res->getPosition());
            }

            if(dist < bestDist)
            {
                bestDist = dist;
                best = temp;
            }

            debutX = best.x - intervalX/4;
            finX = best.x + intervalX/4;
            if(intervalX/(float)4 != intervalX/4 && debutX != finX)
                ++finX;
            debutY = best.y - intervalY/4;
            finY = best.y + intervalY/4;
            if(intervalY/(float)4 != intervalY/4 && debutY != finY)
                ++finY;
        }
        _moyenne = Position(debutX,debutY);
    }
    return _moyenne;
}

float Clan::distance(const Position &pos1, const Position &pos2)
{
    float normal = sqrt((pos1.x-pos2.x)*(pos1.x-pos2.x)+(pos1.y-pos2.y)*(pos1.y-pos2.y));
    float gauche = sqrt((pos1.x-(pos2.x-LARGEUR))*(pos1.x-(pos2.x-LARGEUR))+(pos1.y-pos2.y)*(pos1.y-pos2.y));
    float droite = sqrt((pos1.x-(pos2.x+LARGEUR))*(pos1.x-(pos2.x+LARGEUR))+(pos1.y-pos2.y)*(pos1.y-pos2.y));
    float haut = sqrt((pos1.x-pos2.x)*(pos1.x-pos2.x)+(pos1.y-(pos2.y-HAUTEUR))*(pos1.y-(pos2.y-HAUTEUR)));
    float bas = sqrt((pos1.x-pos2.x)*(pos1.x-pos2.x)+(pos1.y-(pos2.y+HAUTEUR))*(pos1.y-(pos2.y+HAUTEUR)));

    float ret = (bas < haut)?bas:haut;
    ret = (ret < droite)?ret:droite;
    ret = (ret < gauche)?ret:gauche;
    ret = (ret < normal)?ret:normal;

    return ret;
}
