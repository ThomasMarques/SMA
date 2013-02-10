#ifndef PLANETE_H
#define PLANETE_H

#include "commercial.h"
#include <QThread>

#define HAUTEUR 50
#define LARGEUR 50
#define PROBA_RESSOURCE 3 /// En %
#define PROBA_NAISS 1 /// En %age du nombre total de ClanMember
#define NB_PATHFINDER_START 15
#define PROBA_NAISS_MERGE 33 // En %

class Clan;
class Pathfinder;
class ClanMember;

class Planet : public QThread
{
    Q_OBJECT
    private :
        // Matrice contenant les agents et ressources de la map
        Agents ***_map;
        Clan *_clan[2];
        bool ** _fightingMap;
        bool _running;
        unsigned _time;

        // Initialisation de la map
        void init_ressource();

        /// Position de départ des deux clans
        static Position posDepartClan[2];

    protected :
        void run();

    public:
        explicit Planet(QObject *parent = 0);
        ~Planet();
        Agents*** getMap() {return _map;}
        Clan* getClan(unsigned clan) {return _clan[clan];}
        Clan** getClans() {return _clan;}
        bool ***getMapClan();
        bool ** getFightingMap() { return _fightingMap; }
        void naissance();
        unsigned* getT() {return &_time;}
        void stop() {_running = false;}
        bool allResourceBusy();
        ClanMember* getMember(Position xy, unsigned clan);
        Resource* plusProcheRessourceClan(Position inPos,unsigned inClan);



    signals :
        void modelChanged();
};

#endif // PLANETE_H
