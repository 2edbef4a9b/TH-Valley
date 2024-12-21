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

struct Attack {
    int attackdamage_;
    int spellpower_;

    Attack& operator=(const Attack& giv);
    Attack& operator+(const Attack& giv);
};

const Attack casterAttack = {0, 100};
const Attack saberAttack = {100, 0};

struct Defense {
    int phsicaldefense_;
    int spelldefense_;

    Defense& operator=(const Defense& giv);
    Defense& operator+(const Defense& giv);
};

const Defense casterDefense = {10, 10};
const Defense saberDefense = {100, 10};

struct Attribute {
    Attribute();
    bool MakeUMap();
    Attribute(const Exist e,const Attack a,const Defense d);
    std::unordered_map<std::string, Attribute> ChooseOpucation;
    std::unordered_map<std::string, int> chooseId;
    Exist exist_;
    Attack attack_;
    Defense defense_;
    std::vector<std::string> ocupationlist{"caster", "saber"};
    std::vector<std::string> weponlist{"wand", "sword"};
    std::vector<double> experiencelist{1,  100, 100, 200, 300, 300, 500, 500, 500, 900};
};

const float initx = 240;
const float inity = 160;

class Avatar{
public:
    void Set(std::string id);
    Avatar();
    bool inattackzone(Avatar* other) const ;
    void experiencegain(double exp) ;
    void upgrade();
    void existchange();
    bool judgedeath() const ;
    
    friend class avatarScene;


private:
    std::pair<float, float> postion_;
    double experience;
    std::string ocupation_;
    std::string wepon_;
    Attribute attribute;
    double attackzone;
    Bag mybag;
    std::string handy_;
    cocos2d::Sprite* pic;
};

#endif  // AVATAR_H_
