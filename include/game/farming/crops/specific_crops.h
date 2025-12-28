#ifndef SPECIFIC_CROPS_H_
#define SPECIFIC_CROPS_H_

#include "game/farming/crops/crops.h"

namespace th_valley {

// --- Strawberry ---
class Strawberry : public Crops {
public:
    Strawberry() {
        CropName = "Strawberry";
        
        // 状态初始化
        CurrentGrowthStage = 0;
        MaxGrowthStage = 3;
        
        // 生长数据
        GrowthDuration = {50, 50, 50, 50}; 
        
        // --- 表现层配置 ---
        texturePath = "assets/Crops/crops.png";
        
        // 配置每一帧的切片 (从硬编码改为配置)
        frameRects.resize(4);
        frameRects[0].setRect(1, 593, 13, 14);   // Stage 0 (Seed)
        frameRects[1].setRect(36, 597, 8, 8);    // Stage 1
        frameRects[2].setRect(81, 587, 16, 23);  // Stage 2
        frameRects[3].setRect(97, 587, 16, 24);  // Stage 3 (Ripe)

        // 产出配置
        productItemName = "Strawberry"; 
        output_count = 8; // 草莓产量高

        // 属性设置
        isNormal = true;
        GrowthSpeed = 1;
        
        // 种植要求
        SeasonRequirement = {"Spring"};
        SoilRequirement = {"Arable"};
        
        // 注意：这里没有 cocos2d::Sprite::create() 了
    }
};

// --- Carrot ---
class Carrot : public Crops {
public:
    Carrot() {
        CropName = "Carrot";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 3;
        GrowthDuration = {60 * 60 * 24 * 1, 60 * 60 * 24 * 1, 60 * 60 * 24 * 1, 60 * 60 * 24 * 7};
        
        texturePath = "assets/Crops/crops.png";
        
        frameRects.resize(4);
        frameRects[0].setRect(2, 785, 11, 13);
        frameRects[1].setRect(35, 785, 10, 20);
        frameRects[2].setRect(51, 783, 10, 22);
        frameRects[3].setRect(65, 779, 13, 25);
        
        productItemName = "Carrot";
        output_count = 1;

        SeasonRequirement = {"Spring", "Summer", "Autumn"};
    }
};

// --- Potato ---
class Potato : public Crops {
public:
    Potato() {
        CropName = "Potato";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 3;
        GrowthDuration = {50, 50, 50, 50};
        
        texturePath = "assets/Crops/crops.png";
        
        frameRects.resize(4);
        frameRects[0].setRect(130, 54, 10, 7);
        frameRects[1].setRect(180, 50, 10, 9);
        frameRects[2].setRect(208, 45, 14, 19);
        frameRects[3].setRect(225, 42, 14, 22);
        
        productItemName = "Potato";
        output_count = 1;

        SeasonRequirement = {"Spring", "Summer", "Autumn"};
    }
};

// --- Turnip (补全定义) ---
class Turnip : public Crops {
public:
    Turnip() {
        CropName = "Turnip";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 2; // 假设只有3个阶段
        GrowthDuration = {30, 30};
        
        texturePath = "assets/Crops/crops.png";
        
        frameRects.resize(3); 
        // 示例坐标，请根据实际 SpriteSheet 修改
        frameRects[0].setRect(0, 0, 16, 16); 
        frameRects[1].setRect(16, 0, 16, 16);
        frameRects[2].setRect(32, 0, 16, 16);

        productItemName = "Turnip";
        output_count = 1;
        SeasonRequirement = {"Spring"};
    }
};

} // namespace th_valley

#endif // SPECIFIC_CROPS_H_