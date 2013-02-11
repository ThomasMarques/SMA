#include "resource.h"
#include <iostream>

Qt::GlobalColor Resource::color[NB_RESSOURSES] = {Qt::cyan,Qt::yellow,Qt::gray,Qt::red};
const unsigned Resource::probaRes[NB_RESSOURSES] = {10,30,60,100};
int Resource::_NbResource = 0;
Position Resource::_compare = Position();

Resource::Resource(unsigned type, unsigned x, unsigned y, Clan *clan)
{
    _id = ++_NbResource;
    _type = type;
    _position.x = x;
    _position.y = y;
    _clan = clan;
    _viseJedi = false;
    _viseSith = false;
    _ressourcesProduite = 0;
}

bool operator==(const Resource ressource1, const Resource ressource2)
{
    return ressource1._id == ressource2._id;
}
