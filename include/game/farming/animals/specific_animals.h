#ifndef SPECIFIC_ANIMALS_H_
#define SPECIFIC_ANIMALS_H_

#include "game/animals.h"

namespace th_valley {

class Pig : public Animals {
public:
    Pig() {
        AnimalName = "Pig";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 1;
        GrowthDuration = {60 * 60 * 24 * 10}; // 10天
        // GrowthStage = {"Cub", "Mature"}; 
        
        // 产出定义
        Output = {"Truffle"};
        OutputNumber = {1};

        // 动画初始化逻辑
        // 注意：通常建议将 Sprite 创建与逻辑类解耦，但在你的架构中保留在此
        initSprite = cocos2d::Sprite::create("assets/Animals/Pig.png", cocos2d::Rect(0, 0, 32, 32));
        
        // 加载动画帧
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 4; j++) {
                cocos2d::Rect r(j * 32, i * 32, 32, 32);
                auto frame = cocos2d::SpriteFrame::create("assets/Animals/Pig.png", r);
                if (frame) animFrames[i].pushBack(frame);
            }
        }

        scale = {0.6f, 1.0f};

        // 初始状态
        isUnhappy = false; 
        isHungry = false;
        moved = false;
        Happiness = 100; // 建议用数值而不是时间，更容易控制
        Saturation = 100;

        Food = {"Grass"}; // 吃的食物名称
    }
};

// 你可以在这里添加 Cow, Chicken 等其他动物

} // namespace th_valley

#endif // SPECIFIC_ANIMALS_H_