#include "weather.h"
#include "worldtime.h"
#include "agriculture.h"
#include <string>
#include <vector>
#include <random>
#include <utility>

void Crops::UpdateSituation(const std::string &Situation, const bool &Compare,
                            bool &isSituation, const int &toDeath) {
    if (Compare) {
        if (isSituation) {
            isSituation = 0;
            SpecialSituationCount--;
            std::vector<std::string>::iterator it;
            for (it = GrowthSituation.begin(); it != GrowthSituation.end();
                 it++) {
                if (*it == Situation) GrowthSituation.erase(it);
            }
        }
    } else {
        if (!isSituation) {
            isSituation = 1;
            if (toDeathTime == -1) toDeathTime = toDeath;
            else {
                toDeathTime = std::min(toDeathTime, toDeath);
            }
            SpecialSituationCount++;
            if (GrowthSituation[0] == "Normal") GrowthSituation[0] = Situation;
            else {
                GrowthSituation.push_back(Situation);
            }
        }
    }
}

void Crops::CropAutomaticUpdate() {
    // Death Check
    if (GrowthSituation[0] == "Death") return;

    // Watering Check
    WaterRequirement += 1;
    if (CurrentWeather->WeatherType == "Rainy") WaterRequirement = 0;
    UpdateSituation("Drought", WaterRequirement < MaxWaterRequirement,
                    isDrought, 500);

    // Season Check
    bool SeasonCheck = 0;
    for (int i = 0; i < SeasonRequirement.size(); i++) {
        if (SeasonRequirement[i] == CurrentTime->Season) SeasonCheck = 1;
    }
    UpdateSituation("WrongSeason", SeasonCheck, isWrongSeason, 500);

    // Frozen Check
    //CCLOG("Compare: %d\n", CurrentWeather->Temperature);
    UpdateSituation("Frozen", CurrentWeather->Temperature >= MinTemperature,
                    isFrozen, 300);

    // Pest Check
    int PestCheck = 1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 1000);
    int RandomNumber = dist(gen);
    if (RandomNumber == 1 && CurrentWeather->Temperature >= 15 &&
        CurrentWeather->Temperature <= 40) {
        PestCheck = 0;
    }
    UpdateSituation("Pest", PestCheck, isPest, 300);

    // Rot Check
    int RotCheck = !((CurrentGrowthStage == MaxGrowthStage) &&
                   (GrowthDuration[CurrentGrowthStage] <= -500));
    UpdateSituation("Rot", RotCheck, isRot, 0);

    // Deal special situation of the crop
    if (SpecialSituationCount > 0) {
        if (toDeathTime == 0) {
            GrowthSituation = {"Death"};
        }
        toDeathTime--;
        GrowthSpeed = 0;
    } else {
        toDeathTime = -1;
        GrowthSpeed = 1;
        GrowthSituation = {"Normal"};
    }

    // Fertilizer Update
    if (isFertilize) {
        if (FertilizerDuration <= 0) {
            GrowthSpeed = std::min(GrowthSpeed, 1);
            isFertilize = 0;
        }
        else {
            FertilizerDuration--;
        }
    }
    
    // GrowthDuration Update
    if (CurrentGrowthStage < MaxGrowthStage) {
        GrowthDuration[CurrentGrowthStage] -= GrowthSpeed;
        if (GrowthDuration[CurrentGrowthStage] <= 0) {
            CurrentGrowthStage++;
        }
    } else {
        GrowthDuration[CurrentGrowthStage] -= 1;
    }
}

void Crops::CropWatering() { WaterRequirement = 0; }

void Crops::CropFertilize() {
    GrowthSpeed = 2;
    FertilizerDuration = 100;
    isFertilize = 1;
}

void Crops::getTime(WorldTime *Time) { CurrentTime = Time; }

void Crops::getWeather(Weather *weather) { CurrentWeather = weather; }

void CropProduction::CropProductionAutoUpdate() {
    for (int i = 0; i < TotalCrops; i++) {
        Crops *Crop = AllCrops[i];
        Crop->CropAutomaticUpdate();
    }

    // Level Update
    if (TotalPlant >= 5) PlayerCropLevel = 2;
    if (TotalPlant >= 20) PlayerCropLevel = 3;
    if (TotalPlant >= 70) PlayerCropLevel = 4;
    if (TotalPlant >= 500) PlayerCropLevel = 5;
}

void Animals::Stroke() { Happiness += 1000; }

bool Animals::Eat(std::string Eatfood) {
    for (int i = 0; i < Food.size(); i++) {
        if (Food[i] == Eatfood) {
            Saturation += 1000;
            return true;
        }
    }
    return false;
}

void Animals::AnimalAutomaticUpdate() {
    // Happiness Update
    Happiness -= 1;
    Happiness = std::max(0, Happiness);
    if (Happiness == 0)
        isUnhappy = 1;
    else
        isUnhappy = 0;

    // Hunger Update
    Saturation -= 1;
    Saturation = std::max(0, Saturation);
    if (Saturation == 0)
        isHungry = 1;
    else
        isHungry = 0;

    // Growth Update
    if (CurrentGrowthStage < MaxGrowthStage) {
        GrowthDuration[CurrentGrowthStage] -= GrowthSpeed;
        if (GrowthDuration[CurrentGrowthStage] == 0) {
            CurrentGrowthStage += 1;
        }
    }
}

void FarmHouse::addFood(const std::string FoodType, const int number) {
    int isExist = 0;
    for (int i = 0; i < FoodLeft.size(); i++) {
        if (FoodLeft[i].first == FoodType) {
            FoodLeft[i].second += number;
            isExist = 1;
            break;
        }
    }
    if (!isExist) {
        std::pair<std::string, int> newPair;
        newPair.first = FoodType;
        newPair.second = number;
        FoodLeft.push_back(newPair);
    }
}

void FarmHouse::FarmHouseAutomaticUpdate() {
    for (int i = 0; i < TotalAnimals; i++) {
        Animals *Animal = AllAnimals[i];
        // Animal Update
        Animal->AnimalAutomaticUpdate();

        // Animal Eat Left Food
        if (Animal->Saturation <= 5 && !FoodLeft.empty()) {
            std::vector<std::pair<std::string, int> >::iterator it;
            for (it = FoodLeft.begin(); it != FoodLeft.end(); it++)
                if (Animal->Eat(it->first)) {
                    it->second--;
                    if (it->second == 0) FoodLeft.erase(it);
                    break;
                }
        }

        // Animal Output
        if (!Animal->isHungry && !Animal->isUnhappy &&
            Animal->CurrentGrowthStage == Animal->MaxGrowthStage) {
            for (int i = 0; i < Animal->Output.size(); i++) {
                bool isExist = 0;
                for (int j = 0; j < Output.size(); j++) {
                    if (Output[j].first == Animal->Output[i]) {
                        isExist = 1;
                        Output[j].second += Animal->OutputNumber[i];
                        break;
                    }
                }
                if (!isExist) {
                    std::pair<std::string, int> newPair;
                    newPair.first = Animal->Output[i];
                    newPair.second = Animal->OutputNumber[i];
                    Output.push_back(newPair);
                }
            }
        }
    }

    // Level Update
    if (TotalAnimals >= 10) Level = 2;
    if (TotalAnimals >= 20) Level = 3;
}
