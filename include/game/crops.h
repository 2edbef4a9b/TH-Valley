#ifndef CROPS_H_
#define CROPS_H_

#include <string>
#include <vector>
#include "cocos2d.h"

#include "i_time_observer.h"
#include "game/agriculture.h" // Assumed existing base class
#include "game/bag.h"         // Assumed existing
#include "utility/position.h" // Assumed existing

class Weather;   // Forward declaration
class WorldTime; // Forward declaration

class Crops : public Agriculture, public ITimeObserver {
public:
    Crops();
    virtual ~Crops();

    [[nodiscard]] virtual const std::string &getCropName() const;
    void setCurrentGrowthStage(int stage);

    // --- ITimeObserver Implementation ---
    void OnTimeTick(WorldTime* time) override;
    void OnDayChanged(WorldTime* time) override;
    // ------------------------------------

    // Data Members
    std::string CropName;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector<int> GrowthDuration;
    std::vector<std::string> GrowthStage;
    cocos2d::Sprite *CropSprite;
    
    // Status flags
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

    Position position;

    // Logic Methods
    void getWeather(Weather *weather);
    virtual void CropAutomaticUpdate(WorldTime* time);
    virtual void CropWatering();
};

#endif // CROPS_H_