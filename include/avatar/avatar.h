#ifndef AVATAR_H_
#define AVATAR_H_
#include <vector>
#include <string>
#include "cocos2d.h"
#include"creature/creature.h"

class Avatar : public Creature{
public:
    Avatar(std::string name, int wepon, std::string ocupation);
    bool inattackzone(Avatar* other);
    void experiencegain(double exp);
    void upgradeshow();
    void upgrade();
    void doattack(bool isAttack);

private:

    struct Attack {
        int attackdamage_;
        int spellpower_;

        Attack& operator=(Attack& giv);
        Attack& operator+(const Attack& giv);
    };

    const Attack casterAttack = {10, 100};
    const Attack saberAttack = {100.10};
    const Attack shilderAttack = {10, 10};



    struct Defense {
        int phsicaldefense_;
        int spelldefense_;

        Defense& operator=(Defense& giv);
        Defense& operator+(const Defense& giv);
    };

    const Defense casterDefense = {10, 10};
    const Defense saberDefense = {100, 10};
    const Defense shilderDefense = {100, 100};


    // now grade (i=0)
    // when you on grade i,you need experience.at(i) to upgrade to i+1(i>=1)
    std::vector<double> experiencelist{1,100, 100, 200, 300, 300,500, 500, 500, 900};
    double experience;

    std::vector<std::string> ocupationlist{"caster", "saber", "shilder"};
    std::string ocupation_;

    std::vector<std::string> weponlist{"wand","sword","shield"};
    std::string wepon_;

  
    std::string name_;
    Defense defense_;
    Attack attack_;
};

#endif  // AVATAR_H_
