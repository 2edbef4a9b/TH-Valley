#include "game/map_controller.h"

#include <string_view>

#include "game/tiled_map.h"
#include "utility/logger.h"

namespace th_valley {

void MapController::LoadTiledMap(const std::string& tiled_map,
                                 cocos2d::Node* parent) {
    game_tiled_map_ =
        TiledMap::create(kTiledMapPathPrefix + tiled_map + kTiledMapPathSuffix);
    if (game_tiled_map_ == nullptr) {
        Logger::GetInstance().LogError("Failed to load TiledMap: {}",
                                       tiled_map);
        return;
    }
    Logger::GetInstance().LogInfo("Loaded TiledMap: {}", tiled_map);
    parent->addChild(game_tiled_map_, 1);
}

void MapController::TriggerTeleport(const std::string& portal_name) {
    TiledMap::Portal portal(portal_name);
    SaveTiledMap(game_tiled_map_);
    LoadTiledMap(portal.GetToMap(), game_tiled_map_->getParent());
    const auto portal_rect =
        game_tiled_map_->GetPortalRect(portal.GetOppositePorta());

    // Calculate the center of the rectangle
    float center_x = portal_rect.origin.x + portal_rect.size.width / 2.0f;
    float center_y = portal_rect.origin.y + portal_rect.size.height / 2.0f;

    // Set the player's position to the center of the rectangle
    game_tiled_map_->SetPlayerPos(cocos2d::Vec2(center_x, center_y));
}

void MapController::SaveTiledMap(TiledMap* tiled_map) {}

MapController& MapController::GetInstance() {
    static MapController instance;
    return instance;
}

}  // namespace th_valley
