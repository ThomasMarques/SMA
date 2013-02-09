/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri 9. Nov 22:19:55 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QLabel>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "widgetview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget,*widgetTranspa,*widgetFond;
    QPushButton *button_init;
    QPushButton *button_lancer;
    WidgetView *widget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QLabel *labelgauche, *labeldroit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1000, 600);
        MainWindow->setMinimumSize(775,450);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        widgetFond = new QWidget(centralWidget);
        widgetFond->setGeometry(0,0,1000,600);
        widgetFond->setStyleSheet("background: black url('fond.png') no-repeat top center;");
        labelgauche = new QLabel(centralWidget);
        labelgauche->setPixmap(QPixmap("vador.jpg"));
        labeldroit = new QLabel(centralWidget);
        labeldroit->setPixmap(QPixmap("luke.jpg"));
        button_init = new QPushButton(centralWidget);
        button_init->setObjectName(QString::fromUtf8("button_init"));
        button_init->setGeometry(QRect(80, 520, 141, 23));
        button_lancer = new QPushButton(centralWidget);
        button_lancer->setObjectName(QString::fromUtf8("button_lancer"));
        button_lancer->setGeometry(QRect(750, 520, 141, 23));
        widgetTranspa = new QWidget(centralWidget);
        widgetTranspa->setObjectName(QString::fromUtf8("widgetTranspa"));
        widgetTranspa->setStyleSheet("background-color: rgba( 0, 0, 0, 0% );");
        MainWindow->setCentralWidget(centralWidget);
        widget = NULL;
        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        button_init->setText(QApplication::translate("MainWindow", "Initialiser la simulation", 0, QApplication::UnicodeUTF8));
        button_lancer->setText(QApplication::translate("MainWindow", "Lancer la simulation", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
