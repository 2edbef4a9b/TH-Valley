#ifndef SHOP_H_
#define SHOP_H_

#include "bag_gui.h"
#include "bag.h"
#include <map>
#include "agriculture.h"
#include "Animals.h"

class Shop:public BagGUI{
protected:
    std::map<int, cocos2d::Sprite*> IndextoSprite;
    std::map<int, int> IndextoMoney;

public:
    virtual void purchase(int index);
};

class FarmShop : public Shop {
public:
    std::map<int, Animals*> IndextoAnimal;
    FarmShop() {
        ItemSprite* pig = new ItemSprite("Pig", 1, "Pig cub", "assets/Animals/Pig.png",
                         cocos2d::Rect(0, 0, 32, 32));
        bag_->addItem(0, pig);

        IndextoMoney[0] = 500;
    }
};

#endif
