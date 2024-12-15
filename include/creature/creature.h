#ifndef CREATURE_H_
#define CREATURE_H_

#include<string>
#include "cocos2d.h"

class Creature{
public:
    //init:
    Creature();
    Creature& operator= (Creature& other);
    virtual ~Creature();

    // useful function:
    void existchange();
    bool judgedeath();
    virtual void experiencegain() = 0;

    // connect to Exist
    
    // read only:
    double fetchblood();
    double fetchpowerrest();
    int fetchgrade();
    double fetchattackzone();
    // rewrite:
    void setblood(double newblood);
    void setpowerrest(double newpowerrest);
    void setgrade(int newgrade);
    void setzttackzone(double newattackzone);


    std::pair<double, double> postion_;

private:

    struct Exist {
        double blood_;
        double hunger_;
        double thisty_;
        double powerrest_;
        Exist& operator=(Exist& other);
    };

    const Exist casterExist = {1000, 100, 100, 100};
    const Exist saberExist = {1200, 100, 100, 100};
    const Exist shilderExist = {2000, 100, 100, 100};


    int grade_;
    Exist exist_;
    double attackzone;
};

#endif  // CREATURE_H_
