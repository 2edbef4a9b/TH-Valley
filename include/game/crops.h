#ifndef CROPS_H_
#define CROPS_H_

#include <string>
#include <vector>

#include "cocos2d.h"
#include "game/agriculture.h"
#include "utility/position.h"
#include "game/bag.h"

class Crops : public agriculture {
public:
    // Crops Information
    std::string CropName;
    std::string CropType;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector<int> GrowthDuration;
    std::vector<std::string> GrowthStage;
    std::vector<cocos2d::Rect> frameRect;
    cocos2d::Sprite *CropSprite;
    ItemSprite *Fruit;

    // Situation
    std::vector<std::string> GrowthSituation;

    bool isNormal;
    int SpecialSituationCount;
    bool isWrongSeason;
    bool isFrozen;
    int MinTemperature;
    bool isPest;
    bool isDrought;
    bool isRot;
    bool isDeath;
    int toDeathTime;
    bool isFertilize;

    int GrowthSpeed;

    // requirement
    int WaterRequirement;
    int MaxWaterRequirement;
    int FertilizerDuration;
    std::vector<std::string>
        SeasonRequirement;  // Player can only plant in these seasons
    std::vector<std::string>
        SoilRequirement;  // Player can only plant on these types of soil

    // position
    // Map *CurrentMap;
    Position position;

    // Output
    int output;

    void getTime(WorldTime *Time);
    void getWeather(Weather *weather);
    // virtual void ShowCropInfo();
    // virtual void DurationCalculate();
    virtual void CropWatering();
    // virtual void CropHavest();
    virtual void CropFertilize();
    // virtual void CropRemove();
    virtual void CropAutomaticUpdate();
    virtual void UpdateSituation(const std::string &Situation,
                                 const bool &Compare, bool &isSituation,
                                 const int &toDeath);
};

#endif  // CROPS_H_
