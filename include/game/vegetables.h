#ifndef VEGETABLES_H_
#define VEGETABLES_H_

#include "game/crops.h"

class Strawberry : public Crops {
public:
    Strawberry() {
        CropName = "Strawberry";
        CropType = "Fruit";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 3;
        GrowthDuration = {60 * 60 * 24 * 3, 60 * 60 * 24 * 3, 60 * 60 * 24 * 1,
                          60 * 60 * 24 * 7};
        GrowthStage = {"Seed", "Seedling", "Maturation", "Ripened"};
        frameRect.resize(4);
        frameRect[0].setRect(1, 593, 13, 14);
        frameRect[1].setRect(36, 597, 8, 8);
        frameRect[2].setRect(81, 587, 16, 23);
        frameRect[3].setRect(97, 587, 16, 24);
        CropSprite =
            cocos2d::Sprite::create("assets/Crops/crops.png", frameRect[0]);

        GrowthSituation = {"Normal"};
        isNormal = 1;
        SpecialSituationCount = 0;
        isWrongSeason = 0;
        isFrozen = 0;
        isPest = 0;
        isDrought = 0;
        isRot = 0;
        isDeath = 0;
        toDeathTime = -1;
        MinTemperature = 10;
        isFertilize = 0;

        GrowthSpeed = 1;

        output = 8;

        WaterRequirement = 0;
        MaxWaterRequirement = 60 * 60 * 24 * 1;
        FertilizerDuration = 0;
        SoilRequirement = {"Arable"};
        SeasonRequirement = {"Spring"};
    }
};

class Carrot : public Crops {
public:
    Carrot() {
        CropName = "Carrot";
        CropType = "Vegetable";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 3;
        GrowthDuration = {60 * 60 * 24 * 1, 60 * 60 * 24 * 1, 60 * 60 * 24 * 1,
                          60 * 60 * 24 * 7};
        GrowthStage = {"Seed", "Seedling", "Maturation", "Ripened"};
        frameRect.resize(4);
        frameRect[0].setRect(2, 785, 11, 13);
        frameRect[1].setRect(35, 785, 10, 20);
        frameRect[2].setRect(51, 783, 10, 22);
        frameRect[3].setRect(65, 779, 13, 25);
        CropSprite =
            cocos2d::Sprite::create("assets/Crops/crops.png", frameRect[0]);

        GrowthSituation = {"Normal"};
        isNormal = 1;
        SpecialSituationCount = 0;
        isWrongSeason = 0;
        isFrozen = 0;
        isPest = 0;
        isDrought = 0;
        isRot = 0;
        isDeath = 0;
        toDeathTime = -1;
        MinTemperature = 10;
        isFertilize = 0;

        GrowthSpeed = 1;

        output = 1;

        WaterRequirement = 0;
        MaxWaterRequirement = 60 * 60 * 24 * 1;
        FertilizerDuration = 0;
        SoilRequirement = {"Arable"};
        SeasonRequirement = {"Spring", "Summer", "Autumn"};
    }
};

class Potato : public Crops {
public:
    Potato() {
        CropName = "Potato";
        CropType = "Vegetable";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 3;
        // GrowthDuration = {60 * 60 * 24 * 1, 60 * 60 * 24 * 1, 60 * 60 * 24 *
        // 1,
        //                 60 * 60 * 24 * 7};
        GrowthDuration = {50, 50, 50, 50};
        GrowthStage = {"Seed", "Seedling", "Maturation", "Ripened"};
        frameRect.resize(4);
        frameRect[0].setRect(130, 54, 10, 7);
        frameRect[1].setRect(180, 50, 10, 9);
        frameRect[2].setRect(208, 45, 14, 19);
        frameRect[3].setRect(225, 42, 14, 22);
        CropSprite =
            cocos2d::Sprite::create("assets/Crops/crops.png", frameRect[0]);

        GrowthSituation = {"Normal"};
        isNormal = 1;
        SpecialSituationCount = 0;
        isWrongSeason = 0;
        isFrozen = 0;
        isPest = 0;
        isDrought = 0;
        isRot = 0;
        isDeath = 0;
        toDeathTime = -1;
        MinTemperature = 10;
        isFertilize = 0;

        GrowthSpeed = 1;

        output = 1;

        WaterRequirement = 0;
        MaxWaterRequirement = 60 * 60 * 24 * 1;
        FertilizerDuration = 0;
        SoilRequirement = {"Arable"};
        SeasonRequirement = {"Spring", "Summer", "Autumn"};
    }
};

#endif  // VEGETABLES_H_
