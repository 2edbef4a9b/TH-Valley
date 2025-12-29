#ifndef SPECIFIC_ANIMALS_H_
#define SPECIFIC_ANIMALS_H_

#include "game/farming/animals/animals.h"

namespace th_valley {

// ==================== Pig ==================== - Refactored with Factory Pattern
class Pig : public Animals {
public:
    Pig() {
        // --- Logic data --- - Refactored with Factory Pattern
        AnimalName = "Pig";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 1;
        GrowthDuration = 60 * 60 * 24 * 10; // 10 days - Refactored with Factory Pattern
        
        ProduceItemName = "Truffle";
        ProduceCount = 1;
        
        Happiness = 100;
        isHungry = false;
        
        // --- Presentation layer configuration (only configure path, don't create Sprite) --- - Refactored with Factory Pattern
        texturePath = "assets/Animals/Pig.png";
        frameSize = cocos2d::Size(32, 32);
        initialRect = cocos2d::Rect(0, 0, 32, 32);
        scaleRange = {0.6f, 1.0f};
    }
    
    // If Pig has special interaction sounds or behaviors, can override base class methods here - Refactored with Factory Pattern
};

// ==================== Cow ==================== - Refactored with Factory Pattern
class Cow : public Animals {
public:
    Cow() {
        AnimalName = "Cow";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 1;
        GrowthDuration = 60 * 60 * 24 * 5; // 5 days - Refactored with Factory Pattern
        
        ProduceItemName = "Milk";
        ProduceCount = 1;

        Happiness = 100;
        isHungry = false;

        // Assume cow's image is also grid-based - Refactored with Factory Pattern
        texturePath = "assets/Animals/Cow.png"; 
        frameSize = cocos2d::Size(32, 32); // Cow might be larger, e.g., 48x48 - Refactored with Factory Pattern
        initialRect = cocos2d::Rect(0, 0, 32, 32);
        scaleRange = {0.8f, 1.2f};
    }
};

// ==================== Chicken ==================== - Refactored with Factory Pattern
class Chicken : public Animals {
public:
    Chicken() {
        AnimalName = "Chicken";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 1;
        GrowthDuration = 60 * 60 * 24 * 3; // 3 days - Refactored with Factory Pattern
        
        ProduceItemName = "Egg";
        ProduceCount = 1;

        Happiness = 100;
        isHungry = false;

        texturePath = "assets/Animals/Chicken.png";
        frameSize = cocos2d::Size(16, 16); // Chicken is smaller - Refactored with Factory Pattern
        initialRect = cocos2d::Rect(0, 0, 16, 16);
        scaleRange = {0.5f, 0.8f};
    }
};

} // namespace th_valley

#endif // SPECIFIC_ANIMALS_H_