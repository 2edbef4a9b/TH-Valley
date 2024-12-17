#include"avatar/avatar.h"
#include <string>
#include "cocos2d.h"
// operator override
Avatar::Attack& Avatar::Attack::operator=(Avatar::Attack& giv) {
    if (&giv!=this) {
        this->attackdamage_ = giv.attackdamage_;
        this->spellpower_ = giv.spellpower_;
    }
    return *this;
}

Avatar::Attack& Avatar::Attack::operator+(const Avatar::Attack& giv) {
    this->attackdamage_ += giv.attackdamage_;
    this->spellpower_ += giv.spellpower_;
    return *this;
}

Avatar::Defense& Avatar::Defense::operator=( Avatar::Defense& giv) {
    if (&giv != this) {
        this->phsicaldefense_ = giv.phsicaldefense_;
        this->spelldefense_ = giv.spelldefense_;
    }
    return *this;
}

Avatar::Defense& Avatar::Defense::operator+(const Avatar::Defense& giv) {
    this->phsicaldefense_ += giv.phsicaldefense_;
    this->spelldefense_ += giv.spelldefense_;
    return *this;
}

// useful function
bool Avatar::inattackzone(Avatar* other) {
    std::pair<double, double> other_pos(other->postion_);
    double dir = sqrt((other_pos.first - postion_.first) *
                          (other_pos.first - postion_.first) +
                      (other_pos.second - postion_.second) *
                          (other_pos.second - postion_.second));

//double angle = (other_pos.second - postion_.second) /(other_pos.first - postion_.first)

    bool flag = false;
    if (dir < fetchattackzone()) {
        flag = true;
    }
    return flag;
}

void Avatar::experiencegain(double exp)  {
    if (fetchgrade()==10)
    {
        return;
    }
    else
    {
        experience += exp;
        if (experience >= experiencelist.at(int(experiencelist.at(0)))) {

            if (fetchgrade() < 10) {
                experience -= experiencelist.at(int(experiencelist.at(0)));
            }
        }
    }
}

// init:
Avatar::Avatar(std::string id , int choose) {
    wepon_=weponlist.at(choose);
    ocupation_ = ocupationlist.at(choose);

    id_=id;
    choose_ = choose;

}
Avatar::Avatar() {}

// should be override
void Avatar::upgradeshow() {





    CCLOG("upgrade!");
}


void Avatar::upgrade()
{
    setblood(fetchblood()+1000.0);
    setpowerrest(fetchpowerrest() + 100.0);
    setgrade(fetchgrade() + 1);
    experiencelist.at(0) = fetchgrade();
    attack_ = attack_ + Attack{100, 100};
    defense_ = defense_ + Defense{100, 100};
    upgradeshow();
}

// should be override
void Avatar::doattack(bool isAttack,int dir){
    
    CCLOG("ckeck the attack");
}






