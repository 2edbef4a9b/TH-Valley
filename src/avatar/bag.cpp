#include"avatar/bag.h"
#include"cocos2d.h"

bool Bag::isempty() const{
    return currentnum == 0;
}

bool Bag::isfull() const{
    return currentnum == maxnum;
}

void Bag::addobj(std::string key,  ItemSprite* additem) {
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
            idMap.insert(make_pair(key, currentnum - 1));
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
            idMap.erase(key);
            CCLOG("num=0,has earse");
        }
        else{
            CCLOG("the rest is: %d", bag[key]->quantity);
        }
    }
    CCLOG("notfind");
}


Bag::Bag() :currentnum(0),maxnum(100){}




void Bag::BagInit() {
    ItemSprite itemA("Axe", 10, perfect, cocos2d::Sprite::create("tool/tools.png", cocos2d::Rect(304, 159, 17, 17)));
    bag.insert(std::make_pair("Axe", &itemA));
    idMap.insert(std::make_pair("Axe", 0));
    ItemSprite itemB("Hoe", 8, high, cocos2d::Sprite::create("tool/tools.png",cocos2d::Rect(304, 33, 17, 17)));
    bag.insert(std::make_pair("Hoe", &itemB));
    idMap.insert(std::make_pair("Hoe", 1));
    ItemSprite itemC("Fishingrod", 1000, lowest,cocos2d::Sprite::create("tool/tools.png", cocos2d::Rect(268, 310, 21, 15)));
    bag.insert(std::make_pair("Fishingrod", &itemC));
    idMap.insert(std::make_pair("fishingrod", 2));
    ItemSprite itemD("fishingrod", 1000, lowest,cocos2d::Sprite::create("tool/tools.png",cocos2d::Rect(253, 225, 23, 17)));
    bag.insert(std::make_pair("WateringCan", &itemD));
    idMap.insert(std::make_pair("WateringCan", 3));
}

