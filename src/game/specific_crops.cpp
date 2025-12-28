#include "game/specific_crops.h"

namespace th_valley {

Strawberry::Strawberry() {
    CropName = "Strawberry";
    // 假设: 0=种子, 1=发芽, 2=成长, 3=成熟
    MaxGrowthStage = 3; 
    CurrentGrowthStage = 0;
    
    // 初始化生长周期 (例如: 每个阶段需要 2 天)
    GrowthDuration = {2, 2, 2}; 
    
    CropSprite = cocos2d::Sprite::create("crops/strawberry_seed.png");
    
    // 设置属性
    isWrongSeason = false;
    SeasonRequirement = {"Spring"}; // 只能在春天种
    output_count = 1; // 产量
}

Potato::Potato() {
    CropName = "Potato";
    MaxGrowthStage = 4;
    CurrentGrowthStage = 0;
    GrowthDuration = {3, 3, 3, 3}; 
    CropSprite = cocos2d::Sprite::create("crops/strawberry_seed.png");
    isWrongSeason = false;
    SeasonRequirement = {"Spring", "Summer"};
}

Turnip::Turnip() {
    CropName = "Turnip";
    MaxGrowthStage = 2;
    CurrentGrowthStage = 0;
    GrowthDuration = {1, 1}; // 长得快
    isWrongSeason = false;
    CropSprite = cocos2d::Sprite::create("crops/strawberry_seed.png");
    SeasonRequirement = {"Spring"};
}

} // namespace th_valley