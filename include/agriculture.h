#ifndef AGRICULTURE_H_
#define AGRICULTURE_H_

#include "cocos2d.h"
#include "worldtime.h"
#include "weather.h"
#include <vector>
#include <string>

struct Position {
    int x;
    int y;
};
    
class agriculture {
protected:
    WorldTime CurrentTime;
    Weather CurrentWeather;
};

class CropProduction: public agriculture {
    friend class Crops;
protected:
    int PlayerCropLevel;
    int TotalCrops;
private:
    std::vector<Crops> AllCrop;
};

class Crops: public CropProduction {
protected:
    // Crops Information
    std::string CropName;
    std::string CropType;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector < int > GrowthDuration;

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
    std::vector <std::string> SeasonRequirement;// Player can only plant in these seasons
    std::vector <std::string> SoilRequirement;// Player can only plant on these types of soil 

    // position
    Position CropPosition;
public:
    Crops();
    ~Crops();
    virtual void ShowCropInfo();
    virtual void DurationCalculate();
    virtual void CropPlant(const Position &position);
    virtual void CropWatering();
    virtual void CropHavest();
    virtual void CropFertilize();
    virtual void CropRemove();
    virtual void CropAutomaticUpdate();
    virtual void UpdateSituation(const std::string &Situation, const bool &Compare, 
                                 bool &isSituation, const int &toDeath);
};

#endif
