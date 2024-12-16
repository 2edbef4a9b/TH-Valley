#ifndef AGRICULTURE_H_
#define AGRICULTURE_H_

#include "cocos2d.h"
#include "worldtime.h"
#include "weather.h"
#include <vector>
#include <string>
#include <utility>

struct Position {
    int x;
    int y;
};
    
class agriculture {
protected:
    WorldTime *CurrentTime;
    Weather *CurrentWeather;
};

class CropProduction: public agriculture {
protected:
    int PlayerCropLevel;
    int TotalCrops;
};

class Crops: public CropProduction {
public:
    // Crops Information
    std::string CropName;
    std::string CropType;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector < int > GrowthDuration;
    std::vector<std::string> GrowthStage;

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
    Position CropPosition;

    void getTime(WorldTime *Time);
    void getWeather(Weather *weather);
    //virtual void ShowCropInfo();
    //virtual void DurationCalculate();
    //virtual void CropPlant(const Position &position);
    virtual void CropWatering();
    //virtual void CropHavest();
    virtual void CropFertilize();
    //virtual void CropRemove();
    virtual void CropAutomaticUpdate();
    virtual void UpdateSituation(const std::string &Situation, const bool &Compare, 
                                 bool &isSituation, const int &toDeath);
};

class AnimalHusbandry : public agriculture {
protected:
    int PlayerAnimalLevel;
    int TotalAnimals;
};

class Animals : public AnimalHusbandry {
public:
    // Animal Information
    std::string AnimalName;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector<int> GrowthDuration;
    std::vector<std::string> GrowthStage;
    std::vector<std::string> Output;
    std::vector<int> OutputNumber;

    std::vector<std::string> GrowthSituation;

    bool isUnhappy;
    bool isHungry;

    int GrowthSpeed;

    // Requirement
    int Happiness; // ÐÒ¸£¶È
    int Saturation; // ±¥Ê³¶È
    std::vector<std::string> Food;

    void Stroke();
    void Eat(std::string EatFood);
    void RandomMove();
    void AnimalAutomaticUpdate();
    void AnimalShowInfo();
};

class FarmHouse : public AnimalHusbandry {
public:
    int Level;
    std::vector<std::pair<std::string, int> > FoodLeft;
    std::vector<std::pair<std::string, int> > Output;

    FarmHouse() { Level = 1; }
    void InfoShow();
    void addFood(std::string Foodtype, int number);
    std::pair < std::string, int > ProductionOutput();
};

#endif
