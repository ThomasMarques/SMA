#include "strategie.h"
#include "pathfinder.h"
#include "warrior.h"
#include "MersenneTwister.h"
#include "clan.h"

void StrategieExploration::executeStrategie(Clan *clan)
{
    Pathfinder* tmpFind;
    foreach(ClanMember* m, clan->getMembers())
    {
        tmpFind=(Pathfinder*)m;
        if(tmpFind->getVector().x == 0)
        {
            Position tmp((genrand_int32()%10)-5,(genrand_int32()%10)-5);
            if(tmp.x >= 0) tmp.x+=1;
            if(tmp.y >= 0) tmp.y+=1;
            tmpFind->setVector(tmp);
        }
    }
}


void StrategieDefence::executeStrategie(Clan *clan)
{
    Pathfinder* mPath;
    if(!clan->isPromotedPhase())//cas ou défense normale
    {
        foreach(ClanMember* m, clan->getMembers())
        {
            switch(m->getType())
            {
            case pathfinder:
                mPath=(Pathfinder*)m;
                if(!mPath->isPromoted())
                {
                    // Pour les pathfinders restants il couvre la map de manière aléatoire
                    if(mPath->getVector() == Position(0,0))
                    {
                        Position tmp((genrand_int32()%8)-5,(genrand_int32()%8)-5);
                        if(tmp.x >= -1) tmp.x+=3;
                        if(tmp.y >= -1) tmp.y+=3;
                        mPath->setVector(tmp);
                    }
                }
                else
                {
                    if(mPath->isGoingToMerge() && mPath->getObjectif() == mPath->getCurrent() && mPath->getTargetToMerged()->getCurrent() == mPath->getObjectif())
                    {
                        //alors delete 2 eclaireur a cette position et creer un warrior

                        // L'objectif du guerrier est de rejoindre le camp (ressource du clan) le plus proche
                        clan->addMember(mPath->getCurrent(),clan->plusProcheRessource(mPath->getCurrent())->getPosition(),warrior,Alliance(clan->getAlliance()));

                        cout <<"MERGED IN : {"<< mPath->getCurrent().x<<","<< mPath->getCurrent().y<<"}"<< endl;
                        // Suppression des deux pathfinders
                        clan->removeMember(mPath->getTargetToMerged()->getId());
                        mPath->getTargetToMerged()->setPromoted(false);
                        clan->removeMember(mPath->getId());
                    }
                }
                break;
            case warrior://STRATEGIE DEFENCE WARRIOR
                break;
            case robot://STRATEGIE DEFENCE ROBOT
                break;
            }

        }
    }
    else//cas juste après la "course aux ressources"
    {//80% des eclaireurs vont entamer une procedure de mutation en warrior en rejoignant un autre eclaireur
        foreach(ClanMember* m, clan->getMembers())
        {
            if(clan->getNbWarrior()==0 && !((Pathfinder*)m)->isPromoted()&& (genrand_int32()%100)<PROBA_PROMOTED)
            {
                ((Pathfinder*)m)->setPromoted(true);
                m->setVector(Position(1,1));
            }
            else
            {
                if(!((Pathfinder*)m)->isPromoted())
                {
                    m->setObjectif(m->getCurrent());
                }
            }
        }
        clan->setPromotedPhase(false);
    }
}


void StrategieAttaque::executeStrategie(Clan *clan)
{
    Resource* resourceTarget;
    foreach(ClanMember* m, clan->getMembers())
    {
        switch(m->getType())
        {
        case pathfinder://recherche la plus proche ressource enemie pour la capturer
            Pathfinder * mm=(Pathfinder*)m;
            if(m->getCurrent()!= m->getObjectif() || mm->getResourceTargeted() == NULL)
            {
                if(m->getAlliance()== JEDI)
                    resourceTarget=clan->getPlanet()->plusProcheRessourceClan(m->getCurrent(),SITH);
                else
                    resourceTarget=clan->getPlanet()->plusProcheRessourceClan(m->getCurrent(),JEDI);

                m->setObjectif(resourceTarget->getPosition());
                mm->setResourceTargeted(resourceTarget);

                Warrior* w=(Warrior*)m->getFollowerWarrior();
                if(w != NULL)
                    mm->addFollower(w);
            }
            else
            {
                if(m->getCurrent()==m->getObjectif())
                {
                    m->setVector(Position(0,0));
                    mm->catchingRessource();
                }
            }
            break;
        }
    }
}
