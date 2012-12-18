#ifndef RESSOURCE_H
#define RESSOURCE_H

#define NB_RESSOURSES 6

#include <Qt>
#include "MersenneTwister.h"
#include "structure.h"

enum resource_dispo {
    Or, // Objets rare vendu très cher
    Argent // Peut être vendu à bon prix

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

        static int _NbResource;

    public:
        Resource(unsigned type, unsigned x, unsigned y,Clan *clan = NULL);
        Qt::GlobalColor getColor() {return color[_type];}
        Position getPosition() {return _position;}
        Clan* getClan() {return _clan;}
        void setClan(Clan *clan) {_clan = clan;}

        static int GetNbResource() {return _NbResource;}
        static void InitNbResource() {_NbResource = 0;}

        friend bool operator==(const Resource ressource1, const Resource ressource2);

        /// 2 booléens utilisés lors de la phase d'exploration
        bool _viseJedi;
        bool _viseSith;
};

#endif // RESSOURCE_H
