#ifndef COMMERCIAL_H
#define COMMERCIAL_H

#include "resource.h"

class Commercial
{
private :

    static Commercial *instance;
    static unsigned prixResource[4]; ///< Prix d'achat des diff�rentes ressources par le marchant.

public:
    /**
      * \brief Constructeur de la classe commercial permettant les initialisations.
      */
    Commercial();

    /**
      * \brief Ventes de ressources au commercial.
      * \param argentClan Pointeur sur l'argent du clan qui va �tre incr�menter.
      * \param sellRes Ressources que le clan souhaite vendre. Une fois vendu la ressource est remise � zero.
      */
    void sellResources(Clan *clanVendeur, Resource* sellRes);

    /**
      * \brief Permet � un clan d'acheter un robot au commercial.
      * \param argentClan Pointeur sur l'argent du clan qui va �tre d�cr�menter du prix du robot.
      * \param clanAcheteur Clan voulant acqu�rir le robot.
      * \param posRobot Position de naissance du robot.
      */
    bool achatRobot(Clan *clanAcheteur, Position& posRobot);

    /**
      * \brief Permet de r�cup�rer l'instance de Commercial ou de la cr�e si ce n'est pas le cas.
      */
    static Commercial* getInstance();


    static const unsigned  prixRobot; ///< Prix de vente d'un robot.
};

#endif // COMMERCIAL_H
