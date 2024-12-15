#include"avatar/avatar.h"
void avatar::init()
{














}

bool avatar::inattackzone(avatar* other) {
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
avatar::avatar(std::string name, int wepon, std::string ocupation)
{
    name_ = name;
    wepon_=wepon;
    ocupation_ = ocupation;
};
avatar::~avatar() {};
