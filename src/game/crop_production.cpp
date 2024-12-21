#include "game/crop_production.h"

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
