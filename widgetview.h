#ifndef WIDGETVIEW_H
#define WIDGETVIEW_H

#include <QWidget>
#include "planet.h"

class WidgetView : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetView(Agents ***map, unsigned *t, bool ***mapClan,bool ** fightingMap, Clan **clan, QWidget *parent = 0);
    ~WidgetView();
    void paintEvent(QPaintEvent *);

public slots :
    void modelChanged();

private :
    Agents ***_map;
    bool ***_mapClan;
    bool ** _fightingMap;
    Clan **_clan;
    unsigned *_time;


};

#endif // WIDGETVIEW_H
