#ifndef WIDGETVIEW_H
#define WIDGETVIEW_H

#include <QWidget>
#include <QThread>
#include "planet.h"

class WidgetView : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetView(Agents ***map, unsigned *t, bool ***mapClan, QWidget *parent = 0);
    ~WidgetView();
    void paintEvent(QPaintEvent *p);

public slots :
    void modelChanged();

private :
    Agents ***_map;
    bool ***_mapClan;
    unsigned *_time;

    
};

#endif // WIDGETVIEW_H
