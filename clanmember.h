#ifndef CLANMEMBER_H
#define CLANMEMBER_H

#include "structure.h"
#include "planet.h"

enum Member_type {
    warrior,
    pathfinder,
    robot
};

enum Alliance {
    JEDI,
    SITH
};

class ClanMember
{
    private :
        Position _objectif;
        Position _current;
        Planet *_planet;
        Alliance _alliance;
        unsigned _id;
        static unsigned _Count;
        Member_type _type;

        //Deplacement
        Position _vector;
        Position _cpt;

        friend class Pathfinder;
        friend class Robot;
        friend class Warrior;

    protected :
        ClanMember(Position current, Position objectif,Planet* inPlanet,Alliance inAlliance);

    public:
        // Static class to create objects
        static ClanMember* Create(Position current, Position objectif, Member_type type,Planet * inPlanet,Alliance inAlliance);
        Position getObjectif() {return _objectif;}
        Position getCurrent() {return _current;}
        void setObjectif(Position objectif) {_objectif = objectif;}
        void setCurrent(Position current) {_current = current;}
        unsigned getId() {return _id;}
        Member_type getType() {return _type;}
        static unsigned GetCount() {return _Count;}
        Alliance getAlliance() { return _alliance;}

        //Deplacement
        Position& getVector() {return _vector;}
        void setVector(Position inVect) {_vector.x=inVect.x;_vector.y=inVect.y;_cpt = Position(0,0);}
        void moveVector();
        void movePosition();

        virtual void execute() = 0;
};

#endif // CLANMEMBER_H
