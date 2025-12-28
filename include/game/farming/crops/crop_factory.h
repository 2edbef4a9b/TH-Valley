#ifndef CROP_FACTORY_H_
#define CROP_FACTORY_H_

#include "game/crops.h"
#include "game/specific_crops.h" // 包含 Strawberry, Potato, Carrot, Turnip 的定义

namespace th_valley {

// --- 抽象工厂 ---
class CropFactory {
public:
    virtual ~CropFactory() = default;
    virtual Crops* CreateCrop() = 0;
};

// --- 具体工厂 ---

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

} // namespace th_valley

#endif // CROP_FACTORY_H_