#include <cstddef>
#include <chrono>
#include"creature/creature.h"
#include<math.h>

Creature::Exist& Creature::Exist::operator=(Exist& other) {
    if (this != &other) {
        this->blood_ = other.blood_;
        this->hunger_ = other.hunger_;
        this->thisty_ = other.thisty_;
        this->powerrest_ = other.powerrest_;
    }
    return *this;
}

constexpr double Creature::fetchblood() {
    return exist_.blood_;
}

constexpr double Creature::fetchpowerrest() {
    return exist_.powerrest_;
}

constexpr int Creature::fetchgrade() {
    return grade_;
}

constexpr double Creature::fetchattackzone() {
    return attackzone;
}

constexpr void Creature::setblood(double newblood) {
    exist_.blood_ = newblood;
}

constexpr void Creature::setpowerrest(double newpowerrest) {
    exist_.powerrest_ = newpowerrest;
}

constexpr void Creature::setgrade(int newgrade) {
    grade_ = newgrade;
}

constexpr void Creature::setzttackzone(double newattackzone) {
    attackzone = newattackzone;
}

Creature::Creature()
    : grade_{1}, postion_{240, 160}, exist_{0, 0, 0, 0}, attackzone{100} {}
   

Creature& Creature::operator=(Creature& other) {
    if (this!=&other)
    {
        exist_=other.exist_;
        postion_=other.postion_;
        grade_=other.grade_;
    }
    return *this;
}

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

Creature::~Creature() {};

