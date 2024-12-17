#include"avatar/bag.h"
#include"cocos2d.h"

bool Bag::isempty() const{
    return currentnum == 0;
}
bool Bag::isfull() const{
    return currentnum == maxnum;
}
void Bag::addobj(std::string key,int addnum) {
    if (isfull()){
        CCLOG("bag is full,please check the bag");
    }
    else {
        auto pos = bag.find(key);
        if (pos != bag.end()) {
            int temp = bag[key];
            bag[key] = temp + addnum;
            CCLOG("add successfully");
        }
        else{
            bag.insert(make_pair(key, addnum));
            CCLOG("insert successfully");
            currentnum++;
        }

    }
}
int Bag::count(std::string key) {
    auto pos = bag.find(key);
    if (pos != bag.end()) {  
        CCLOG("find");
        return bag[key];
    } else {
        CCLOG("not find");
        return -1;
    }
}
void Bag::reduce(std::string key) {
    auto pos = bag.find(key);
    if (pos != bag.end()) {
        bag[key] -= 1;
        if (bag[key] == 0) {
            bag.erase(key);
            currentnum--;
            CCLOG("num=0,has earse");
        }
        else{
            CCLOG("the rest is: %d", bag[key]);
        }
    }
    CCLOG("notfind");
}
void Bag::openbag() {











    CCLOG("open");
}
Bag::Bag() :currentnum(0),maxnum(100){}

