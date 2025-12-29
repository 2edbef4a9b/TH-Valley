#ifndef CROPS_H_
#define CROPS_H_

#include <string>
#include <vector>
#include "cocos2d.h"

#include "game/core/i_time_observer.h"
#include "game/farming/crops/agriculture.h" 
#include "game/core/position.h"

// Forward declaration - Refactored with Factory Pattern
class WorldTime;

namespace th_valley {

class Crops : public Agriculture, public ITimeObserver {
public:
    Crops(); 
    virtual ~Crops();

    // --- ITimeObserver implementation ---
    void OnTimeTick(WorldTime* time) override;
    void OnDayChanged(WorldTime* time) override;
    // --------------------------

    // --- Core logic attributes ---
    std::string CropName;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector<int> GrowthDuration; // Number of ticks required for each stage - Refactored with Factory Pattern
    
    // Output attributes - Refactored with Factory Pattern
    std::string productItemName;
    int output_count;
    
    // Planting requirements - Refactored with Factory Pattern
    std::vector<std::string> SeasonRequirement;
    std::vector<std::string> SoilRequirement;

    // --- Status flags ---
    bool isWatered;
    bool isDead;
    bool isFrozen;
    bool isFertilize;
    bool isNormal; // Used for special status judgment - Refactored with Factory Pattern
    int GrowthSpeed;

    // --- Presentation layer configuration (View Config) --- - Refactored with Factory Pattern
    // In constructor only set these data, don't create Sprite - Refactored with Factory Pattern
    std::string texturePath; 
    std::vector<cocos2d::Rect> frameRects; // Store slice areas for each growth stage - Refactored with Factory Pattern
    
    // --- Presentation layer instance ---
    cocos2d::Sprite *CropSprite; 
    Position position;

    // --- Core methods ---
    
    // [New] Initialize view: key to decoupling - Refactored with Factory Pattern
    // Called during Manager PlantCrop - Refactored with Factory Pattern
    virtual void InitializeView(cocos2d::Node* parent);

    void setCurrentGrowthStage(int stage);
    
    // Logic update - Refactored with Factory Pattern
    virtual void CropAutomaticUpdate(WorldTime* time); 
    
    // Interaction behavior - Refactored with Factory Pattern
    virtual void CropWatering(); 
    virtual void Harvest();
    
    const std::string& getCropName() const { return CropName; }
};

} // namespace th_valley

#endif // CROPS_H_