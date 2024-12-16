#ifndef CROPS_H_
#define CROPS_H_

#include "agriculture.h"

class Strawberry: public Crops {
public:
    Strawberry() {
        CropName = "Strawberry";
        CropType = "Fruit";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 3;
        GrowthDuration = {300, 300, 300};
        GrowthStage = {"Seed", "Seedling", "Maturation", "Ripened"};

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

        WaterRequirement = 0;
        MaxWaterRequirement = 500;
        FertilizerDuration = 0;
        SoilRequirement = {"ArableLand"};
        SeasonRequirement = {"Spring"};
    }
};

#endif
