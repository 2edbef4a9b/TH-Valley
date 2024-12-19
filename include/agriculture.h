#ifndef AGRICULTURE_H_
#define AGRICULTURE_H_

#include "cocos2d.h"
#include "worldtime.h"
#include "weather.h"
#include <vector>
#include <string>
#include <utility>
#include <map>
#include "map.h"
#include "position.h"

class Map;
class agriculture {
protected:
    WorldTime *CurrentTime;
    Weather *CurrentWeather;
};

class Crops: public agriculture {
public:
    // Crops Information
    std::string CropName;
    std::string CropType;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector < int > GrowthDuration;
    std::vector<std::string> GrowthStage;
    std::vector<cocos2d::Rect> frameRect;

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
    std::vector <std::string> SeasonRequirement;// Player can only plant in these seasons
    std::vector <std::string> SoilRequirement;// Player can only plant on these types of soil 

    // position
    // Map *CurrentMap;
    Position position;

    void getTime(WorldTime *Time);
    void getWeather(Weather *weather);
    //virtual void ShowCropInfo();
    //virtual void DurationCalculate();
    virtual void CropWatering();
    //virtual void CropHavest();
    virtual void CropFertilize();
    //virtual void CropRemove();
    virtual void CropAutomaticUpdate();
    virtual void UpdateSituation(const std::string &Situation, const bool &Compare, 
                                 bool &isSituation, const int &toDeath);
};

class CropProduction : public agriculture {
public:
    int PlayerCropLevel;
    int TotalCrops;
    int TotalPlant;
    std::vector<Crops *> AllCrops;

    CropProduction() {
        PlayerCropLevel = 1;
        TotalCrops = 0;
        TotalPlant = 0;
    }
    void CropProductionAutoUpdate();
};

class Animals : public agriculture {
public:
    // Animal Information
    std::string AnimalName;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector<int> GrowthDuration;
    std::vector<std::string> GrowthStage;
    std::vector<std::string> Output;
    std::vector<int> OutputNumber;

    bool isUnhappy;
    bool isHungry;

    int GrowthSpeed;

    // Requirement
    int Happiness; // ÐÒ¸£¶È
    int Saturation; // ±¥Ê³¶È
    std::vector<std::string> Food;

    Position position;

    virtual void Stroke();
    virtual bool Eat(std::string EatFood);
    virtual void RandomMove();
    virtual void AnimalAutomaticUpdate();

    virtual void AnimalShowInfo();
};

class FarmHouse : public agriculture {
public:
    int TotalAnimals;
    int Level;
    std::vector<std::pair<std::string, int> > FoodLeft;
    std::vector<std::pair<std::string, int> > Output;
    std::vector<Animals *> AllAnimals;
    std::map<Position, Animals *> AnimalPosition;
    
    FarmHouse() {
        Level = 1;
        TotalAnimals = 0;
    }
    void InfoShow();
    void addFood(const std::string FoodType, const int number);
    std::pair<std::string, int> ProductionOutput();
    void FarmHouseAutomaticUpdate();
};

#endif
