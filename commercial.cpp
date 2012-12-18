#include "commercial.h"
#include <clan.h>

unsigned Commercial::prixRessource[4] = {500,200,100,50}; ///< Prix d'achat des différentes ressources par le marchant.
unsigned Commercial::prixRobot = 1000; ///< Prix de vente d'un robot.

Commercial::Commercial()
{

}

bool Commercial::achatRobot(unsigned *argentClan, Clan *clanAcheteur, Position& posRobot)
{
    bool ret = (*argentClan >= prixRobot);
    if(ret)
    {
        (*argentClan) -= prixRobot;
        clanAcheteur->addMember(posRobot,posRobot,Member_type(2),clanAcheteur->getAlliance());
    }
    return re;
}


void Commercial::sellResources(unsigned *argentClan, Resource *sellRes)
{
    (*argentClan) += prixResource[sellRes->getType()];

    delete sellRes;
}
