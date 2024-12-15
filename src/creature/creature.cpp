#include <cstddef>
#include <string>
#include <chrono>
#include"creature/creature.h"
#include<math.h>
//operator override

Exist& Exist::operator=(Exist& other)
{
    if (this != &other)
    {
        this->blood_ = other.blood_;
        this->hunger_ = other.hunger_;
        this->thisty_ = other.thisty_;
        this->powerrest_= other.powerrest_;
    }
    return *this;
}

Attack& Attack::operator=(Attack& giv) {
    this->attackdamage_ = giv.attackdamage_;
    this->spellpower_ = giv.spellpower_;
    return *this;
}

Defense& Defense::operator=(Defense& giv) {
    this->physicalresistance_ = giv.physicalresistance_;
    this->spellresistance_ = giv.spellresistance_;
    return *this;
}


Creature::Creature()
    : grade_{1}, postion_{100, 200}, exist_{}, attack_{100, 100} {}
   

Creature& Creature::operator=(Creature& other) {
    if (this!=&other)
    {
        exist_=other.exist_;
        attack_=other.attack_;
        postion_=other.postion_;
        grade_=other.grade_;
    }
    return *this;
}
Creature::~Creature(){}
void Creature::existchange()
{
    exist_.hunger_ *= 0.99;
    exist_.thisty_ *= 0.99;
}
bool Creature::judgedeath() 
{
    bool flag=false;
    double exp = 0.00001;
    if (abs(exist_.blood_) < exp)
    {
        flag = true;
    }
    return flag;
}

