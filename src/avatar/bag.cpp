#include"avatar/bag.h"
#include"cocos2d.h"

bool Bag::isempty() const{
    return currentnum == 0;
}

bool Bag::isfull() const{
    return currentnum == maxnum;
}

void Bag::addobj(std::string key, ItemSprite* additem) {
    if (isfull()){
        CCLOG("bag is full,please check the bag");
    }
    else {
        auto pos = bag.find(key);
        if (pos != bag.end()) {
            int temp = bag[key]->quantity;
            bag[key]->quantity = temp + additem->quantity;
            CCLOG("add successfully");
        }
        else{
            bag.insert(make_pair(key, additem));
            CCLOG("insert successfully");
            currentnum++;
        }

    }
}

ItemSprite* Bag::myfind(std::string key) {
    auto pos = bag.find(key);
    if (pos != bag.end()) {  
        CCLOG("find");
        return bag[key];
    } else {
        CCLOG("not find");
        return NULL;
    }
}
void Bag::reduce(std::string key) {
    auto pos = bag.find(key);
    if (pos != bag.end()) {
        bag[key]->quantity -= 1;
        if (bag[key]->quantity == 0) {
            bag.erase(key);
            currentnum--;
            CCLOG("num=0,has earse");
        }
        else{
            CCLOG("the rest is: %d", bag[key]->quantity);
        }
    }
    CCLOG("notfind");
}

void Bag::openbag() {
    CCLOG("open");
}

Bag::Bag() :currentnum(0),maxnum(100){}

