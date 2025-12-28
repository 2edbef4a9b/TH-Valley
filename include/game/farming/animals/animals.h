#ifndef ANIMALS_H_
#define ANIMALS_H_

#include <string>
#include "cocos2d.h"

#include "i_time_observer.h"
#include "game/agriculture.h"
#include "utility/position.h"

namespace th_valley {

class Animals : public Agriculture, public ITimeObserver {
public:
    Animals();
    virtual ~Animals();

    // --- ITimeObserver 实现 ---
    void OnTimeTick(WorldTime* time) override;
    void OnDayChanged(WorldTime* time) override;
    // --------------------------

    std::string AnimalName;
    bool isHungry;
    bool isUnhappy;
    int Happiness;
    Position position;
    cocos2d::Sprite* AnimalSprite; // 自己的精灵

    // 个体行为
    virtual void Stroke();
    virtual bool Eat(const std::string& EatFood);
    virtual void AnimalAutomaticUpdate(WorldTime* time);
};

} // namespace th_valley

#endif // ANIMALS_H_