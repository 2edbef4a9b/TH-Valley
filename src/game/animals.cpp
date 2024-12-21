#include "game/animals.h"

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
