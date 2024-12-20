#ifndef AVATAR_H_
#define AVATAR_H_
#include <vector>
#include <string>
#include <unordered_map>
#include "cocos2d.h"
#include"avatar/bag.h"
#include"avatar/avatar_scene.h"

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
    std::vector<double> experiencelist{10,  100, 100, 200, 300, 300, 500, 500, 500, 900};

};

enum class STATE {
    move,
    cut,
    cultivate,
    fish,
    mine,
    attack,
    none
};


class Avatar : public cocos2d::Sprite {
public:
    Avatar(std::string id);
    Avatar();
    bool inattackzone(Avatar* other);
    void experiencegain(double exp) ;
    void upgrade();
    void existchange();
    bool judgedeath();
    friend class avatarScene;

private:
    std::pair<double, double> postion_;
    double experience;
    std::string ocupation_;
    std::string wepon_;
    std::string id_;  
    Attribute attribute;
    double attackzone;
    Bag mybag;
    int grade_;
    STATE state;
};

#endif  // AVATAR_H_
