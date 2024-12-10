#include <cstddef>
#include <string>
#include <chrono>
#include"creature/creature.h"
#include<math.h>

Creature::Exist& Creature::Exist::operator=(Exist& other)
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
Creature::Exist::Exist()
    : blood_{1000},
      hunger_{100},
      thisty_{100},
      powerrest_{100}{}



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
bool Creature::inattackzone(Creature* other) {
    std::pair<int, int> other_pos(other->postion_);
    double dir = sqrt((other_pos.first - postion_.first) *
                          (other_pos.first - postion_.first) +
                      (other_pos.second - postion_.second) *
                          (other_pos.second - postion_.second));
    bool flag = false;
    double exp = 0.00001;
    if (abs(dir) < exp) {
        flag = true;
    }
    return flag;
}
