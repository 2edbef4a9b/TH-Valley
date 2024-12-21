#ifndef CROP_PRODUCTION_H_
#define CROP_PRODUCTION_H_

#include <vector>

#include "game/agriculture.h"

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

#endif  // CROP_PRODUCTION_H_
