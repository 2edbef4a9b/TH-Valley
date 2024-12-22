#ifndef CROP_PRODUCTION_H_
#define CROP_PRODUCTION_H_

#include <vector>

#include "game/agriculture.h"
#include "game/crops.h"

class CropProduction : public Agriculture {
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
