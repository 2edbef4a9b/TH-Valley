#ifndef CREATURE_H_
#define CREATURE_H_

#include<string>
#include "cocos2d.h"

struct Attack
{
    double attackdamage_;
    double spellpower_;
};

class Creature
{
public:
    Creature();
    Creature& operator= (Creature& other);
    virtual ~Creature();
    virtual void init() = 0;
    virtual void move() = 0;
    void existchange();
    bool judgedeath();
    bool inattackzone(Creature* other);
private:
    class Exist {
    private:
        double blood_;
        double hunger_;
        double thisty_;
        double powerrest_;
    public:
        friend class Creature;
        Exist& operator=(Exist& other);
        Exist();
    };
    Exist exist_;
    Attack attack_;
    std::pair<int, int> postion_;
    int grade_;
};

#endif  // CREATURE_H_
