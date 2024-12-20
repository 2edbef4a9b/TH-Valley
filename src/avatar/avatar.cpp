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



void Avatar::Differ(std::string handy) {
    
    if (handy == "Axe") {
        state = STATE::cut;
    }
    if (handy == "Hoe") {
        state = STATE::cultivate;
    }
    if (handy == "Fishingrod") {
        state = STATE::fish;
    }
    if (handy == "Draft") {
        state = STATE::mine;
    }
    if (handy == "wand" || handy == "sword") {
        state = STATE::attack;
    }
    CCLOG("======");
    CCLOG("do it");
    CCLOG("======");

}
bool Attribute::MakeUMap() {

    Attribute attCaster(casterExist, casterAttack, casterDefense);
    ChooseOpucation.insert(make_pair(ocupationlist.at(0), attCaster));
    chooseId.insert(make_pair(ocupationlist.at(0),0));

    Attribute attSaber(saberExist, saberAttack, saberDefense);
    ChooseOpucation.insert(make_pair(ocupationlist.at(1), attSaber));
    chooseId.insert(make_pair(ocupationlist.at(1), 1));

    return true;
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
        if (experience >= attribute.experiencelist.at(int(attribute.experiencelist.at(0)))) {

            if (grade_ < 10) {
                experience -= attribute.experiencelist.at(int(attribute.experiencelist.at(0)));
            }
        }
    }
}

// init:
Avatar::Avatar(std::string id) {
    bool flag = attribute.MakeUMap();
    state = STATE::none;
    handy_ = "Axe";
    if (flag) {
        wepon_ = attribute.weponlist.at(attribute.chooseId[id]);
        ocupation_ = id;
        attribute = attribute.ChooseOpucation.at(id);
        mybag.BagInit();
        
        CCLOG("init successfully");
    }
    CCLOG("init unseccessfully");

}
Avatar::Avatar() {}



void Avatar::upgrade()
{
    attribute.exist_.blood_ += 1000.0f;
    attribute.exist_.powerrest_ += 100.0f;
    grade_ += 1;
    attribute.experiencelist.at(0) = grade_;
    attribute.attack_ = attribute.attack_ + Attack{100, 100};
    attribute.defense_ = attribute.defense_ + Defense{100, 100};
}

void Avatar::existchange() {
    attribute.exist_.hunger_ *= 0.99;
    attribute.exist_.thisty_ *= 0.99;
}

bool Avatar::judgedeath() {
    bool flag = false;
    double exp = 0.00001;
    if (abs(attribute.exist_.blood_) < exp) {
        flag = true;
    }
    return flag;
}


