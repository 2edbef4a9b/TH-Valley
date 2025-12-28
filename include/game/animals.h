// animals.h
#ifndef ANIMALS_H_
#define ANIMALS_H_

#include <string>
#include <vector>
#include "cocos2d.h"

#include "i_time_observer.h"
#include "game/agriculture.h"
#include "utility/position.h"

class WorldTime;

class Animals : public Agriculture, public ITimeObserver {
public:
    Animals();
    virtual ~Animals();

    // --- ITimeObserver Implementation ---
    void OnTimeTick(WorldTime* time) override;
    void OnDayChanged(WorldTime* time) override;
    // ------------------------------------

    std::string AnimalName;
    bool isHungry;
    bool isUnhappy;
    int Happiness;
    Position position;

    virtual void Stroke();
    virtual bool Eat(std::string EatFood);
    virtual void AnimalAutomaticUpdate(WorldTime* time);
};

#endif // ANIMALS_H_