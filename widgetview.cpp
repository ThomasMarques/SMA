#include "widgetview.h"
#include "QPainter"
#include "clan.h"
#include <iostream>
#include <sstream>

WidgetView::WidgetView(Agents ***map, unsigned *t, bool ***mapClan, QWidget *parent) : QWidget(parent)
{
    _map = map;
    _t = t;
    _mapClan = mapClan;
}

WidgetView::~WidgetView()
{
    delete[] _mapClan;
}


void WidgetView::paintEvent(QPaintEvent *p)
{
    std::stringstream ss;
    unsigned hauteur = (this->height()-20)/HAUTEUR;
    unsigned largeur = this->width()/LARGEUR;
    unsigned i,j,decal;
    bool diffZero;
    QPainter painter(this);
    QPen pen(Qt::white,1);
    painter.setPen(pen);

    ss << "Temps " << *_t;
    painter.drawText(this->width()/2-50,0,250,15,0,QString(ss.str().c_str()));

    QFont font;
    font.setPointSize(6);
    painter.setFont(font);

    painter.drawRect(0,15,largeur*LARGEUR,hauteur*HAUTEUR);

    for(i = 0 ; i < HAUTEUR ; ++i)
    {
        for(j = 0 ; j < LARGEUR ; ++j)
        {
            decal = 1;
            if(_mapClan[0][i][j])
                painter.fillRect(j*largeur+1,i*hauteur+16,largeur/2,hauteur-1,Qt::blue);
            if(_mapClan[1][i][j])
                painter.fillRect((j+1)*largeur-largeur/2,i*hauteur+16,largeur/2,hauteur-1,Qt::darkRed);

            diffZero = _map[i][j]->jedi.nbEclaireur > 0 || _map[i][j]->sith.nbEclaireur > 0 || _map[i][j]->jedi.nbGuerrier>0 || _map[i][j]->sith.nbGuerrier>0;

            if(diffZero)
            {
                ss.str("");
                ss << _map[i][j]->jedi.nbGuerrier << _map[i][j]->jedi.nbEclaireur << "-" << _map[i][j]->sith.nbEclaireur << _map[i][j]->sith.nbGuerrier;
            }
            if(_map[i][j]->res != NULL)
            {
                painter.setPen(_map[i][j]->res->getColor());
                painter.drawRect(j*largeur,i*hauteur+15,largeur,hauteur);
                painter.setPen(pen);
                if(_map[i][j]->res->getClan() != NULL)
                {
                    if(_map[i][j]->res->getClan()->getAlliance() == JEDI)
                    {
                        painter.fillRect((j+1)*largeur-3,i*hauteur+16,3,hauteur-1,Qt::blue);
                        painter.drawLine((j+1)*largeur-3,i*hauteur+16,(j+1)*largeur-3,(i+1)*hauteur+14);
                    }
                    else
                    {
                        painter.fillRect((j+1)*largeur-3,i*hauteur+16,3,hauteur-1,Qt::red);
                        painter.drawLine((j+1)*largeur-3,i*hauteur+16,(j+1)*largeur-3,(i+1)*hauteur+14);
                    }
                }
                if(diffZero)
                {
                    painter.drawText(j*largeur+3,i*hauteur+17,largeur-4,hauteur-4,0,QString(ss.str().c_str()));
                }
            }
            else
            {
                if(diffZero)
                    painter.drawText(j*largeur+3,i*hauteur+17,largeur-4,hauteur-4,0,QString(ss.str().c_str()));
            }

        }
    }

    painter.end();
}

void WidgetView::modelChanged()
{
    repaint();
}
