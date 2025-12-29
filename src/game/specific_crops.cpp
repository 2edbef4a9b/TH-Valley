#include "game/specific_crops.h"

namespace th_valley {

Strawberry::Strawberry() {
    CropName = "Strawberry";
    // Assume: 0=seed, 1=sprouting, 2=growth, 3=mature - Refactored with Factory Pattern
    MaxGrowthStage = 3;
    CurrentGrowthStage = 0;

    // Initialize growth cycle (e.g.: each stage needs 2 days) - Refactored with Factory Pattern
    GrowthDuration = {2, 2, 2};
    
    CropSprite = cocos2d::Sprite::create("crops/strawberry_seed.png");

    // Set properties - Refactored with Factory Pattern
    isWrongSeason = false;
    SeasonRequirement = {"Spring"}; // Can only be planted in Spring - Refactored with Factory Pattern
    output_count = 1; // Yield - Refactored with Factory Pattern
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
    GrowthDuration = {1, 1}; // Grows fast - Refactored with Factory Pattern
    isWrongSeason = false;
    CropSprite = cocos2d::Sprite::create("crops/strawberry_seed.png");
    SeasonRequirement = {"Spring"};
}

} // namespace th_valley