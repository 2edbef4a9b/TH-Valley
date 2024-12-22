#ifndef FARM_HOUSE_H_
#define FARM_HOUSE_H_

#include <utility>

#include "game/agriculture.h"
#include "game/animals.h"
#include "utility/position.h"

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

extern FarmHouse GlobalFarmHouse;

#endif  // FARM_HOUSE_H_
