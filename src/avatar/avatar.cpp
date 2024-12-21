#include <string>
#include "cocos2d.h"
#include"avatar/avatar_application.h"
#include"avatar/avatar.h"

// operator override
Attack& Attack::operator=(const Attack& giv) {
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

Defense& Defense::operator = (const Defense& giv) {
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

Exist& Exist::operator=(const Exist& other) {
    if (this != &other) {
        this->blood_ = other.blood_;
        this->hunger_ = other.hunger_;
        this->thisty_ = other.thisty_;
        this->powerrest_ = other.powerrest_;
    }
    return *this;
}

Attribute::Attribute() {}

Attribute::Attribute(const Exist e, const Attack a, const Defense d) {
    exist_ = e;
    attack_ = a;
    defense_ = d;
}

bool Attribute::MakeUMap() {

    Attribute attCaster(casterExist, casterAttack, casterDefense);
    ChooseOpucation.insert(make_pair(ocupationlist.at(0), attCaster));
    chooseId.insert(make_pair(ocupationlist.at(0),0));

    Attribute attSaber(saberExist, saberAttack, saberDefense);
    ChooseOpucation.insert(make_pair(ocupationlist.at(1), attSaber));
    chooseId.insert(make_pair(ocupationlist.at(1), 1));
    CCLOG(" Makemap successfully");
    return true;
}

// useful function
bool Avatar::inattackzone(Avatar* other) const {
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
    if (attribute.experiencelist[0] < 10)
    {
        experience += exp;
        if (experience >=
            attribute.experiencelist[attribute.experiencelist[0]]) {
            attribute.experiencelist[0]++;
            if (attribute.experiencelist[0] < 10) {
                experience -=attribute.experiencelist[attribute.experiencelist[0]];
            } else {
                CCLOG("full grade");
            }
        }
    }
}

// init:// Êä³öÂÒÂë
void Avatar::Set(std::string id) {
    CCLOG("==================================================");
    CCLOG("ocupation is : %s", id);
    if (id == "caster" || id == "saber") {
        ocupation_ = id;
        CCLOG("ocupation is : %s", ocupation_);
        CCLOG("ocupation is : %s", id);
        auto posid = attribute.chooseId.find(id);
        if (posid != attribute.chooseId.end()) {
            wepon_ = attribute.weponlist[attribute.chooseId[id]];
            CCLOG("wepon is : %s", wepon_);
            CCLOG("wepon is : %s",attribute.weponlist[attribute.chooseId[id]]);
        } else {
            CCLOG("id error");
        }
        attribute.exist_ = attribute.ChooseOpucation[id].exist_;
        CCLOG("blood = %lf,hunger = %lf,thirsty = %lf,power = %lf",
              attribute.exist_.blood_, attribute.exist_.hunger_,
              attribute.exist_.thisty_, attribute.exist_.powerrest_);
        attribute.defense_ = attribute.ChooseOpucation[id].defense_;
        CCLOG("phsicaldefense_ = %d,spelldefense_ = %d",attribute.defense_.phsicaldefense_, attribute.defense_.spelldefense_);
        attribute.attack_ = attribute.ChooseOpucation[id].attack_;
        CCLOG("phsicaldamage__ = %d,spellpower_ = %d",attribute.attack_.attackdamage_,attribute.attack_.spellpower_);
        CCLOG(" set successfully");
    }
    CCLOG("==================================================");
}
Avatar::Avatar() {
   

    postion_.first = initx;
    postion_.second = inity;
    bool flag = attribute.MakeUMap();
    handy_ = "Axe";
    attackzone = 10;
    experience = 0;
    mybag.BagInit();
}



void Avatar::upgrade()
{
    attribute.exist_.blood_ += 1000.0f;
    attribute.exist_.powerrest_ += 100.0f;
    attribute.experiencelist[0]++;
    attribute.attack_ = attribute.attack_ + Attack{100, 100};
    attribute.defense_ = attribute.defense_ + Defense{100, 100};
}

void Avatar::existchange() {
    attribute.exist_.hunger_ *= 0.99;
    attribute.exist_.thisty_ *= 0.99;
}

bool Avatar::judgedeath() const  {
    bool flag = false;
    double exp = 0.00001;
    if (abs(attribute.exist_.blood_) < exp) {
        flag = true;
    }
    return flag;
}


