#include "game/crops.h"

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
            if (toDeathTime == -1)
                toDeathTime = toDeath;
            else {
                toDeathTime = std::min(toDeathTime, toDeath);
            }
            SpecialSituationCount++;
            if (GrowthSituation[0] == "Normal")
                GrowthSituation[0] = Situation;
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
    // CCLOG("Compare: %d\n", CurrentWeather->Temperature);
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
        } else {
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
