#ifndef ANIMAL_FACTORY_H_
#define ANIMAL_FACTORY_H_

#include "game/animals.h"
#include "game/specific_animals.h" // 包含 Pig, Cow, Chicken 的定义

namespace th_valley {

// --- 抽象工厂 ---
class AnimalFactory {
public:
    virtual ~AnimalFactory() = default;
    virtual Animals* CreateAnimal() = 0;
};

// --- 具体工厂 ---

class PigFactory : public AnimalFactory {
public:
    Animals* CreateAnimal() override { return new Pig(); }
};

// 如果有牛或鸡，也在这里添加
/*
class CowFactory : public AnimalFactory {
public:
    Animals* CreateAnimal() override { return new Cow(); }
};
*/

} // namespace th_valley

#endif // ANIMAL_FACTORY_H_