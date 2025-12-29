#ifndef FARM_SYSTEM_FACADE_H_
#define FARM_SYSTEM_FACADE_H_

#include <string>
#include "cocos2d.h"

#include "game/worldtime.h"
#include "game/weather.h"
#include "game/crop_manager.h"
#include "game/animal_manager.h"
#include "game/statistics_manager.h"
#include "utility/position.h"

namespace th_valley {

class FarmSystemFacade {
public:
    FarmSystemFacade();
    ~FarmSystemFacade();

    // Disable copying - Refactored with Facade Pattern
    FarmSystemFacade(const FarmSystemFacade&) = delete;
    FarmSystemFacade& operator=(const FarmSystemFacade&) = delete;

    // --- Initialization - Refactored with Facade Pattern ---
    // map_root_node: Parent node for rendering Sprites (usually TiledMap or ObjectLayer) - Refactored with Facade Pattern
    void InitSystem(cocos2d::Node* map_root_node);

    // --- Game Loop - Refactored with Facade Pattern ---
    // Called in TiledMap::update - Refactored with Facade Pattern
    void UpdateGame(float dt);

    // --- Player Interaction Interface (Core) - Refactored with Facade Pattern ---
    // Called when player clicks on the map - Refactored with Facade Pattern
    // grid_pos: Clicked grid coordinates - Refactored with Facade Pattern
    // world_pos: Clicked world pixel coordinates (used for animal click detection) - Refactored with Facade Pattern
    // selected_tool: Current held tool name (e.g. "Hoe", "Seeds", "WateringCan") - Refactored with Facade Pattern
    void OnInteract(const Position& grid_pos, const cocos2d::Vec2& world_pos, const std::string& selected_tool);

    // Dedicated planting interface (typically called internally by OnInteract, can also be used for debugging) - Refactored with Facade Pattern
    bool PlantCrop(const Position& pos, const std::string& seed_name);

    // --- Data Save/Load Interface - Refactored with Facade Pattern ---
    void SaveData(const std::string& save_file);
    void LoadData(const std::string& save_file);

    // --- Subsystem Accessors (for UI use) - Refactored with Facade Pattern ---
    WorldTime* GetWorldTime() const { return time_system_; }
    Weather* GetWeather() const { return weather_system_; }
    StatisticsManager* GetStatistics() const { return statistics_manager_; }
    // CropManager/AnimalManager typically don't need to be exposed to UI, unless for debugging collision boxes - Refactored with Facade Pattern

private:
    // Environment system references (using global singleton or injection) - Refactored with Facade Pattern
    WorldTime* time_system_{nullptr};
    Weather* weather_system_{nullptr};

    // Subsystem managers (owned by Facade and lifecycle managed) - Refactored with Facade Pattern
    CropManager* crop_manager_{nullptr};
    AnimalManager* animal_manager_{nullptr};
    StatisticsManager* statistics_manager_{nullptr};

    // Internal helper logic - Refactored with Facade Pattern
    void HandleCropInteraction(const Position& pos, const std::string& tool);
    void HandleAnimalInteraction(const cocos2d::Vec2& world_pos, const std::string& tool);
};

} // namespace th_valley

#endif // FARM_SYSTEM_FACADE_H_