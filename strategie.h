#ifndef ISTRATEGIE_H
#define ISTRATEGIE_H

#define PROBA_PROMOTED 60 //en %

class Clan;

class Strategie
{
    public:
        virtual void executeStrategie(Clan *) = 0;
};

class StrategieExploration : public Strategie
{
    public:
        void executeStrategie(Clan *clan);
};

class StrategieDefence : public Strategie
{
    public:
        void executeStrategie(Clan *clan);
};

class StrategieAttaque : public Strategie
{
    public:
        void executeStrategie(Clan *clan);
};

#endif // ISTRATEGIE_H
