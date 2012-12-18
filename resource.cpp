#include "resource.h"
#include <iostream>

Qt::GlobalColor Resource::color[NB_RESSOURSES] = {Qt::red,Qt::green,Qt::cyan,Qt::yellow};
int Resource::_NbResource = 0;

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
