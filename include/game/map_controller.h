#ifndef MAP_CONTROLLER_H_
#define MAP_CONTROLLER_H_

#include <sys/stat.h>

#include <string_view>

#include "2d/CCNode.h"
#include "cocos2d.h"
#include "game/tiled_map.h"

namespace th_valley {

class TiledMap;

class MapController {
public:
    MapController(const MapController& other) = delete;
    MapController& operator=(const MapController& other) = delete;
    MapController(MapController&& other) = delete;
    MapController& operator=(MapController&& other) = delete;

    void LoadTiledMap(const std::string& tiled_map, cocos2d::Node* parent);
    void SaveTiledMap(TiledMap* tiled_map);
    void TriggerTeleport(const std::string& portal_name);
    void LoadRain(const std::string& tiled_map);

    static MapController& GetInstance();

private:
    MapController() = default;
    ~MapController() = default;

    const std::string kTiledMapPathPrefix = "assets/maps/";
    const std::string kTiledMapPathSuffix = ".tmx";

    TiledMap* game_tiled_map_;
};

}  // namespace th_valley

#endif  // MAP_CONTROLLER_H_
