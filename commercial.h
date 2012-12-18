#ifndef COMMERCIAL_H
#define COMMERCIAL_H

#include <resource.h>
#include <robot.h>

class Commercial
{
private :


    static unsigned prixResource[]; ///< Prix d'achat des diff�rentes ressources par le marchant.
    static unsigned prixRobot; ///< Prix de vente d'un robot.

public:
    Commercial();

    /**
      * \brief Ventes de ressources au commercial.
      * \param argentClan Pointeur sur l'argent du clan qui va �tre incr�menter.
      * \param sellRes Ressources que le clan souhaite vendre. Une fois vendu la ressource est supprimer.
      */
    void sellResources(unsigned *argentClan, Resource* sellRes);

    /**
      * \brief Permet � un clan d'acheter un robot au commercial.
      * \param argentClan Pointeur sur l'argent du clan qui va �tre d�cr�menter du prix du robot.
      * \param clanAcheteur Clan voulant acqu�rir le robot.
      * \param posRobot Position de naissance du robot.
      */
    bool achatRobot(unsigned *argentClan, Clan *clanAcheteur, Position& posRobot);
};

#endif // COMMERCIAL_H
