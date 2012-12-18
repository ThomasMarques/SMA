#ifndef CLAN_H
#define CLAN_H

#include <QVector>
#include "resource.h"
#include "planet.h"
#include "clanmember.h"
#include "strategie.h"

using namespace std;

#define WARRIOR_VIEW 3
#define PATHFINDER_VIEW 5
#define ROBOT_VIEW 3

class Clan
{
    private :
        Strategie *_strategie;
        QVector<Resource*> _resources;
        QVector<ClanMember*> _members;
        // Matrice contenant les agents et ressources de la map connue
        Agents ***_mapConnue;
        Position _posDepart;
        Planet *_planet;
        unsigned _nbWarrior;
        unsigned _nbWarriorAdv;
        unsigned _nbPathfinder;
        unsigned _nbPathfinderAdv;
        bool **_mapVisitee;
        unsigned _alliance;
        bool _promotedPhase;

        // Les stratégies que le clan peut adopter
        static StrategieExploration _Se;
        static StrategieAttaque _Sa;
        static StrategieDefence _Sd;


    public :
        Clan(Position posDepart, Planet *planete, unsigned alliance);
        ~Clan();
        void init();
        void addRessource(Resource *resource) {resource->setClan(this);_resources.push_back(resource);}
        void removeRessource(Resource *ressource);
        void addMember(Position current, Position objectif,Member_type type,Alliance inAlliance);
        void removeMember(unsigned inId);
        Agents*** getMapConnue() {return _mapConnue;}
        bool** getMapVisitee() {return _mapVisitee;}
        void initMapVisitee();
        void execute();
        QVector<ClanMember*>& getMembers() {return _members;}
        QVector<Resource*>& getResources() {return _resources;}
        unsigned getAlliance() {return _alliance;}
        unsigned getNbWarrior() { return _nbWarrior;}
        Planet* getPlanet() { return _planet; }
        bool isPromotedPhase() { return _promotedPhase; }
        void setPromotedPhase(bool inPhase) { _promotedPhase=inPhase; }
        Resource* plusProcheRessource(Position inPos);
};

#endif // CLAN_H
