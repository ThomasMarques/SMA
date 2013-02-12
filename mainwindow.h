#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgetview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *);

public slots :
    void on_button_init_clicked();
    void on_button_lancer_clicked();
    void on_button_pause_clicked();
    
private:
    Ui::MainWindow *ui;
    Planet *_planet;
    float _zoom;
};

#endif // MAINWINDOW_H
