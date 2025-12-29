#ifndef ANIMAL_FACTORY_H_
#define ANIMAL_FACTORY_H_

#include "game/farming/animals/animals.h"
#include "game/farming/animals/specific_animals.h"

namespace th_valley {

// --- Abstract Factory - Refactored with Abstract Factory Pattern ---
class AnimalFactory {
public:
    virtual ~AnimalFactory() = default;
    virtual Animals* CreateAnimal() = 0;
};

// --- Concrete Factories - Refactored with Factory Pattern ---

class PigFactory : public AnimalFactory {
public:
    Animals* CreateAnimal() override { return new Pig(); }
};

class CowFactory : public AnimalFactory {
public:
    Animals* CreateAnimal() override { return new Cow(); }
};

class ChickenFactory : public AnimalFactory {
public:
    Animals* CreateAnimal() override { return new Chicken(); }
};

}  // namespace th_valley

#endif  // ANIMAL_FACTORY_H_
