#ifndef ANIMALS_H_
#define ANIMALS_H_

#include <string>
#include <vector>
#include "cocos2d.h"

#include "game/core/i_time_observer.h" // Assume path has been adjusted - Refactored with Factory Pattern
#include "game/farming/crops/agriculture.h"
#include "game/core/position.h"

namespace th_valley {

class Animals : public Agriculture, public ITimeObserver {
public:
    Animals();
    virtual ~Animals();

    // --- ITimeObserver implementation ---
    void OnTimeTick(WorldTime* time) override;
    void OnDayChanged(WorldTime* time) override;
    // --------------------------

    // --- Core logic data ---
    std::string AnimalName;
    bool isHungry;
    bool isUnhappy;
    int Happiness;
    Position position;
    
    // Growth data - Refactored with Factory Pattern
    int CurrentGrowthStage;
    int MaxGrowthStage;
    int GrowthDuration; // Total growth duration (seconds or ticks) - Refactored with Factory Pattern
    
    // Output data - Refactored with Factory Pattern
    std::string ProduceItemName; // Output item name - Refactored with Factory Pattern
    int ProduceCount;

    // --- Presentation layer configuration (View Config) --- - Refactored with Factory Pattern
    // These data are set in specific subclass constructors, but don't immediately load images - Refactored with Factory Pattern
    std::string texturePath;    // Image path - Refactored with Factory Pattern
    cocos2d::Size frameSize;    // Size of single animation frame (e.g. 32x32) - Refactored with Factory Pattern
    cocos2d::Rect initialRect;  // Initial standing frame area - Refactored with Factory Pattern
    std::vector<float> scaleRange; // {min, max}

    // --- Presentation layer instance ---
    cocos2d::Sprite* AnimalSprite = nullptr; 
    cocos2d::Vector<cocos2d::SpriteFrame*> animFrames[5]; // Store animation frames - Refactored with Factory Pattern

    // --- Core methods ---
    
    // [New] Initialize view: key to decoupling - Refactored with Factory Pattern
    // Called during Manager Spawn, not in constructor - Refactored with Factory Pattern
    virtual void InitializeView(cocos2d::Node* parent);

    // Individual behavior - Refactored with Factory Pattern
    virtual void Stroke();
    virtual bool Eat(const std::string& EatFood);
    virtual void AnimalAutomaticUpdate(WorldTime* time);

protected:
    // Helper: Cut animation frames based on texturePath and frameSize - Refactored with Factory Pattern
    void GenerateFrames();
};

} // namespace th_valley

#endif // ANIMALS_H_