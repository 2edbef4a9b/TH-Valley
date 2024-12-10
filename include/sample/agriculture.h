#ifndef AGRICULTURE_H_
#define AGRICULTURE_H_

#include "cocos2d.h"
#include "sample/worldtime.h"
#include "sample/weather.h"
#include <vector>

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
protected:
    int PlayerCropLevel;
    int TotalCrops;
};

class Crops : public CropProduction {
protected:
    // Crops Information
    std::string CropName;
    std::string CropType;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector < int > GrowthDuration; 
    std::vector <std::string> GrowthSituation;

    // requirement
    int WaterRequirement;
    int FertilizerRequirement;
    std::string SoilRequirement;

    // position
    Position CropPosition;
public:
    Crops();
    ~Crops();
    void ShowCropInfo();
    virtual void DurationCalculate();
    virtual void CropPlant(const Position &position);
    virtual void CropWatering();
    virtual void CropHavest();
    virtual void CropFertilize();
    virtual void Cropremove();
};

#endif
