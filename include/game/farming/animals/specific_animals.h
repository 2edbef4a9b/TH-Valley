#ifndef SPECIFIC_ANIMALS_H_
#define SPECIFIC_ANIMALS_H_

#include "game/farming/animals/animals.h"

namespace th_valley {

// ==================== 猪 ====================
class Pig : public Animals {
public:
    Pig() {
        // --- 逻辑数据 ---
        AnimalName = "Pig";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 1;
        GrowthDuration = 60 * 60 * 24 * 10; // 10天
        
        ProduceItemName = "Truffle";
        ProduceCount = 1;
        
        Happiness = 100;
        isHungry = false;
        
        // --- 表现层配置 (只配置路径，不创建 Sprite) ---
        texturePath = "assets/Animals/Pig.png";
        frameSize = cocos2d::Size(32, 32);
        initialRect = cocos2d::Rect(0, 0, 32, 32);
        scaleRange = {0.6f, 1.0f};
    }
    
    // 如果 Pig 有特殊的交互声音或行为，可以在这里重写基类方法
};

// ==================== 牛 ====================
class Cow : public Animals {
public:
    Cow() {
        AnimalName = "Cow";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 1;
        GrowthDuration = 60 * 60 * 24 * 5; // 5天
        
        ProduceItemName = "Milk";
        ProduceCount = 1;

        Happiness = 100;
        isHungry = false;

        // 假设牛的图也是网格状的
        texturePath = "assets/Animals/Cow.png"; 
        frameSize = cocos2d::Size(32, 32); // 牛可能更大，例如 48x48
        initialRect = cocos2d::Rect(0, 0, 32, 32);
        scaleRange = {0.8f, 1.2f};
    }
};

// ==================== 鸡 ====================
class Chicken : public Animals {
public:
    Chicken() {
        AnimalName = "Chicken";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 1;
        GrowthDuration = 60 * 60 * 24 * 3; // 3天
        
        ProduceItemName = "Egg";
        ProduceCount = 1;

        Happiness = 100;
        isHungry = false;

        texturePath = "assets/Animals/Chicken.png";
        frameSize = cocos2d::Size(16, 16); // 鸡比较小
        initialRect = cocos2d::Rect(0, 0, 16, 16);
        scaleRange = {0.5f, 0.8f};
    }
};

} // namespace th_valley

#endif // SPECIFIC_ANIMALS_H_