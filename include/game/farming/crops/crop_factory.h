#ifndef CROP_FACTORY_H_
#define CROP_FACTORY_H_

#include "game/crops.h"
#include "game/specific_crops.h"  // Contains definitions for Strawberry, Potato, Carrot, Turnip - Refactored with Factory Pattern

namespace th_valley {

// --- Abstract Factory - Refactored with Abstract Factory Pattern ---
class CropFactory {
public:
    virtual ~CropFactory() = default;
    virtual Crops* CreateCrop() = 0;
};

// --- Concrete Factories - Refactored with Factory Pattern ---

class StrawberryFactory : public CropFactory {
public:
    Crops* CreateCrop() override { return new Strawberry(); }
};

class PotatoFactory : public CropFactory {
public:
    Crops* CreateCrop() override { return new Potato(); }
};

class CarrotFactory : public CropFactory {
public:
    Crops* CreateCrop() override { return new Carrot(); }
};

class TurnipFactory : public CropFactory {
public:
    Crops* CreateCrop() override { return new Turnip(); }
};

}  // namespace th_valley

#endif  // CROP_FACTORY_H_
