#include <string>
#include "cocos2d.h"
#include"avatar/avatar_application.h"
#include"avatar/avatar.h"
// operator override
Attack& Attack::operator=(Attack& giv) {
    if (&giv!=this) {
        this->attackdamage_ = giv.attackdamage_;
        this->spellpower_ = giv.spellpower_;
    }
    return *this;
}

Attack& Attack::operator+(const Attack& giv) {
    this->attackdamage_ += giv.attackdamage_;
    this->spellpower_ += giv.spellpower_;
    return *this;
}

Defense& Defense::operator = (Defense& giv) {
    if (&giv != this) {
        this->phsicaldefense_ = giv.phsicaldefense_;
        this->spelldefense_ = giv.spelldefense_;
    }
    return *this;
}

Defense& Defense::operator + (const Defense& giv) {
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
    bool flag = false;
    if (dir < attackzone) {
        flag = true;
    }
    return flag;
}

void Avatar::experiencegain(double exp)  {
    if (grade_==10)
    {
        return;
    }
    else
    {
        experience += exp;
        if (experience >= experiencelist.at(int(experiencelist.at(0)))) {

            if (grade_ < 10) {
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



void Avatar::upgrade()
{
    exist_.blood_ += 1000.0f;
    exist_.powerrest_ += 100.0f;
    grade_ += 1;
    experiencelist.at(0) = grade_;
    attack_ = attack_ + Attack{100, 100};
    defense_ = defense_ + Defense{100, 100};
}

// should be override
void Avatar::doattack(bool isAttack,int dir){
    
    CCLOG("ckeck the attack");
}




Exist& Exist::operator=(Exist& other) {
    if (this != &other) {
        this->blood_ = other.blood_;
        this->hunger_ = other.hunger_;
        this->thisty_ = other.thisty_;
        this->powerrest_ = other.powerrest_;
    }
    return *this;
}


void Avatar::existchange() {
    exist_.hunger_ *= 0.99;
    exist_.thisty_ *= 0.99;
}

bool Avatar::judgedeath() {
    bool flag = false;
    double exp = 0.00001;
    if (abs(exist_.blood_) < exp) {
        flag = true;
    }
    return flag;
}


