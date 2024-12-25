#ifndef CITIZEN_H_
#define CITIZEN_H_

#include <string>
#include <vector>

#include "2d/CCNode.h"
#include "cocos2d.h"
#include "game/agriculture.h"
#include "utility/position.h"
#include "game/bag.h"

class Citizen {
public:
    std::string CitizenName;
    bool InfoOpen = false;

    // Animation
    cocos2d::Vector<cocos2d::SpriteFrame *> animFrames[10];
    cocos2d::Sprite *initSprite;
    std::vector<float> scale;
    bool moved = false;

    Position position;

    // virtual void purchase();
    /* virtual void RandomMove();*/

    /*virtual void AnimalShowInfo();*/
    void ShowTalkBox(cocos2d::Node *parent, Bag* bag_, std::string name);
};

#endif
