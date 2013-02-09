#ifndef RESSOURCE_H
#define RESSOURCE_H

#define NB_RESSOURSES 4
#define CATCHING_TIME 5

#include <Qt>
#include "MersenneTwister.h"
#include "structure.h"

enum resource_dispo {
    Diamond,///< Ressou rare tr�s convoit�e du commer�ant.
    Gold,   ///< Ressource rare vendu tr�s chere.
    Silver, ///< Ressource limit�, peut �tre vendu � bon prix.
    Bronze  ///< Ressource abondante, vendu � bas prix.
};

class Clan;

class Resource
{
    private :
        unsigned _id;
        unsigned _type;
        static Qt::GlobalColor color[];
        Position _position;
        Clan *_clan;
        unsigned _ressourcesProduite; ///< Nombre de ressources produites depuis l'acquisition de la ressources.

        static int _NbResource;

    public:
        Resource(unsigned type, unsigned x, unsigned y,Clan *clan = NULL);

        unsigned getType() {return _type;}
        Qt::GlobalColor getColor() {return color[_type];}
        Position getPosition() {return _position;}
        Clan* getClan() {return _clan;}
        unsigned getRessourcesProduite() {return _ressourcesProduite;}
        void RAZRessourcesProduite() {_ressourcesProduite = 0;}

        void setClan(Clan *clan) {_clan = clan; _ressourcesProduite = 0;}
        void incRessourcesProduite() {++_ressourcesProduite;}
        static int GetNbResource() {return _NbResource;}
        static void InitNbResource() {_NbResource = 0;}

        friend bool operator==(const Resource ressource1, const Resource ressource2);

        /// 2 bool�ens utilis�s lors de la phase d'exploration
        bool _viseJedi;
        bool _viseSith;


        static const unsigned probaRes[];
};

#endif // RESSOURCE_H
