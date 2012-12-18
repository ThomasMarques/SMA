#include "commercial.h"
#include "clan.h"
#include <iostream>

Commercial* Commercial::instance = NULL;
unsigned Commercial::prixResource[4] = {500,200,100,50}; ///< Prix d'achat des différentes ressources par le marchant.
unsigned Commercial::prixRobot = 1000; ///< Prix de vente d'un robot.

Commercial::Commercial()
{

}

bool Commercial::achatRobot(Clan *clanAcheteur, Position &posRobot)
{
    bool ret = (clanAcheteur->getArgent() >= prixRobot);
    if(ret)
    {
        clanAcheteur->decArgent(prixRobot);
        clanAcheteur->addMember(posRobot,posRobot,Member_type(2),Alliance(clanAcheteur->getAlliance()));
    }
    return ret;
}


void Commercial::sellResources(Clan *clanVendeur, Resource *sellRes)
{
    clanVendeur->incArgent(sellRes->getRessourcesProduite()*prixResource[sellRes->getType()]);
    std::cout << sellRes->getRessourcesProduite() << std::endl;
    sellRes->RAZRessourcesProduite();

    delete sellRes;
}

Commercial* Commercial::getInstance()
{
    if(!instance)
        instance = new Commercial();
    return instance;
}
