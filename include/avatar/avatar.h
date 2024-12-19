#ifndef AVATAR_H_
#define AVATAR_H_
#include <vector>
#include <string>
#include <unordered_map>
#include "cocos2d.h"
#include"avatar/bag.h"

struct Exist {
    double blood_;
    double hunger_;
    double thisty_;
    double powerrest_;
    Exist& operator=(const Exist& other);
};

const Exist casterExist = {1000, 100, 100, 100};
const Exist saberExist = {1200, 100, 100, 100};
const Exist shilderExist = {2000, 100, 100, 100};



struct Attack {
    int attackdamage_;
    int spellpower_;

    Attack& operator=(const Attack& giv);
    Attack& operator+(const Attack& giv);
};

const Attack casterAttack = {10, 100};
const Attack saberAttack = {100, 10};
const Attack shilderAttack = {10, 10};



struct Defense {
    int phsicaldefense_;
    int spelldefense_;

    Defense& operator=(const Defense& giv);
    Defense& operator+(const Defense& giv);
};

const Defense casterDefense = {10, 10};
const Defense saberDefense = {100, 10};
const Defense shilderDefense = {100, 100};



std::vector<std::string> ocupationlist{"caster", "saber", "shilder"};
std::vector<std::string> weponlist{"wand", "sword", "shield"};
std::vector<double> experiencelist{1,   100, 100, 200, 300,
                                   300, 500, 500, 500, 900};




struct Attribute {
    Attribute();
    void MakeUMap();
    Attribute(const Exist e,const Attack a,const Defense d);
    int id;
    std::unordered_map<std::string, Attribute*> ChooseOpucation;
    Exist exist_;
    Attack attack_;
    Defense defense_;

};



class Avatar : public cocos2d::Sprite {
public:
    Avatar(std::string id);
    Avatar();

    bool inattackzone(Avatar* other);
    void experiencegain(double exp) ;

    void upgrade();
    void doattack(bool isAttack,int dir) ;

    

    // useful function:
    void existchange();
    bool judgedeath();
   
    
    

    std::pair<double, double> postion_;

private:
  
    double experience;
    std::string ocupation_;
    std::string wepon_;
    std::string id_;  
    Attribute attribute;
    double attackzone;
    Bag mybag;
    int grade_;
};

#endif  // AVATAR_H_
