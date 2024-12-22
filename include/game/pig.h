#ifndef PIG_H_
#define PIG_H_

#include "game/animals.h"

class Pig : public Animals {
public:
    Pig() {
        AnimalName = "Pig";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 1;
        GrowthDuration = {60 * 60 * 24 * 10};
        GrowthStage = {"Cub", "Mature"};
        Output = {"Truffle"};
        OutputNumber = {1};

        initSprite = cocos2d::Sprite::create("assets/Animals/Pig.png",
                                             cocos2d::Rect(0, 0, 32, 32));
        // initSprite->setScale(0.6);
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 4; j++) {
                cocos2d::Rect frameRect(j * 32, i * 32, 32, 32);
                auto frame = cocos2d::SpriteFrame::create(
                    "assets/Animals/Pig.png", frameRect);
                if (frame) animFrames[i].pushBack(frame);
            }
        }

        scale = {0.6, 1};

        isUnhappy = true;
        isHungry = true;
        moved = false;

        GrowthSpeed = 1;

        Happiness = 60 * 60 * 24 * 3;
        Saturation = 60 * 60 * 12;
        Food = {"Grass"};
    }
};

#endif  // PIG_H_
