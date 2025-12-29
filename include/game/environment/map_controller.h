#ifndef MAP_CONTROLLER_H_
#define MAP_CONTROLLER_H_

#include <string>
#include <string_view>
#include <vector>

#include "cocos2d.h"
#include "math/Vec2.h"
#include "utility/position.h"

// Forward declaration of TiledMap to avoid circular dependency from including tiled_map.h - Refactored with Facade Pattern
namespace th_valley {
    class TiledMap; 
}

namespace th_valley {

class MapController {
public:
    // Disable copying and assignment - Refactored with Facade Pattern
    MapController(const MapController&) = delete;
    MapController& operator=(const MapController&) = delete;
    MapController(MapController&&) = delete;
    MapController& operator=(MapController&&) = delete;

    // Get singleton - Refactored with Facade Pattern
    static MapController& GetInstance();

    // --- Core logic ---

    // Load and switch maps - Refactored with Facade Pattern
    void LoadTiledMap(const std::string& tiled_map_name, cocos2d::Node* parent);

    // Save current map state (delegated to TiledMap -> Facade) - Refactored with Facade Pattern
    void SaveCurrentMap();

    // Trigger teleport (usually called after TiledMap detects collision) - Refactored with Facade Pattern
    void TriggerTeleport(const std::string& portal_name);

    // --- Player data management ---
    void SetPlayerPos(cocos2d::Vec2 pos);
    cocos2d::Vec2 GetPlayerPos() const;

    void LoadPlayerInfo();
    void SavePlayerInfo();

    std::string GetPlayerName() const { return player_name_; }
    std::string GetPlayerMap() const { return player_map_; }

    // --- Auxiliary functions ---
    void LoadRainEffect(const std::string& tiled_map_name);

    // Get current TiledMap pointer - Refactored with Facade Pattern
    TiledMap* GetTiledMap() const { return game_tiled_map_; }

private:
    MapController() = default;
    ~MapController() = default;

    const std::string kTiledMapPathPrefix = "assets/maps/";
    const std::string kTiledMapPathSuffix = ".tmx";

    TiledMap* game_tiled_map_{nullptr};

    std::string player_name_;
    std::string player_map_; 
    cocos2d::Vec2 player_pos_;
};

}  // namespace th_valley

#endif  // MAP_CONTROLLER_H_