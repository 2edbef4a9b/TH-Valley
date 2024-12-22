#include "game/farm_house.h"

#include <string>
#include <utility>
#include <vector>

#include "game/animals.h"

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
