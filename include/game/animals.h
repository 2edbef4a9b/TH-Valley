#ifndef ANIMALS_H_
#define ANIMALS_H_

#include <string>
#include <vector>

#include "cocos2d.h"
#include "game/agriculture.h"
#include "utility/position.h"

class Animals : public Agriculture {
public:
    // Animal Information
    std::string AnimalName;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector<int> GrowthDuration;
    std::vector<std::string> GrowthStage;
    std::vector<std::string> Output;
    std::vector<int> OutputNumber;
    bool InfoOpen = false;

    // Animation
    cocos2d::Vector<cocos2d::SpriteFrame *> animFrames[10];
    cocos2d::Sprite *initSprite;
    std::vector<float> scale;

    bool isUnhappy;
    bool isHungry;

    int GrowthSpeed;
    bool moved;

    // Requirement
    int Happiness;
    int Saturation;
    std::vector<std::string> Food;

    Position position;

    virtual void Stroke();
    virtual bool Eat(std::string EatFood);
    /* virtual void RandomMove();*/
    virtual void AnimalAutomaticUpdate();

    /*virtual void AnimalShowInfo();*/
};

#endif  // ANIMALS_H_
