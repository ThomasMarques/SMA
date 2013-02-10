#ifndef ISTRATEGIE_H
#define ISTRATEGIE_H

#define PROBA_PROMOTED 60 //en %
#include <QString>

class Clan;

class Strategie
{

    public:
        virtual void executeStrategie(Clan *) = 0;
        virtual QString getStrategieName() { return "Strategie"; }
};

class StrategieExploration : public Strategie
{
    public:
        void executeStrategie(Clan *clan);
        QString getStrategieName() { return "Exploration"; }
};

class StrategieDefence : public Strategie
{
    public:
        void executeStrategie(Clan *clan);
        QString getStrategieName() { return "Defence"; }
};

class StrategieAttaque : public Strategie
{
    public:
        void executeStrategie(Clan *clan);
        QString getStrategieName() { return QString("Attack"); }
};

#endif // ISTRATEGIE_H
