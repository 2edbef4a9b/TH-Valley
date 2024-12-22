#include "game/map_controller.h"

#include <string_view>

#include "game/tiled_map.h"
#include "utility/logger.h"

namespace th_valley {

void MapController::LoadTiledMap(const std::string& tiled_map,
                                 cocos2d::Node* parent, ToolBar* map_toolbar_, BagGUI* map_bag_gui_) {
    game_tiled_map_ =
        TiledMap::create(kTiledMapPathPrefix + tiled_map + kTiledMapPathSuffix);
    game_tiled_map_->MapToolBar = map_toolbar_;
    game_tiled_map_->MapBag = map_bag_gui_;
    if (game_tiled_map_ == nullptr) {
        Logger::GetInstance().LogError("Failed to load TiledMap: {}",
                                       tiled_map);
        return;
    }
    Logger::GetInstance().LogInfo("Loaded TiledMap: {}", tiled_map);
    parent->addChild(game_tiled_map_, 1);
}

void MapController::TriggerTeleport(const std::string& portal_name,
                                    ToolBar* map_toolbar_,
                                    BagGUI* map_bag_gui_) {
    const TiledMap::Portal portal(portal_name);

    // Check if the map file exists.
    std::string map_path =
        kTiledMapPathPrefix + portal.GetToMap() + kTiledMapPathSuffix;
    if (!cocos2d::FileUtils::getInstance()->isFileExist(map_path)) {
        Logger::GetInstance().LogError("Map file not found: {}", map_path);
        return;
    }

    // Save and remove the current map.
    SaveTiledMap(game_tiled_map_);
    auto* parent = game_tiled_map_->getParent();
    if (parent == nullptr) {
        Logger::GetInstance().LogError("Parent node is null");
        return;
    }
    game_tiled_map_->unscheduleUpdate();
    parent->removeChild(game_tiled_map_);

    // Load the new map and set the player's position.
    LoadTiledMap(portal.GetToMap(), parent, map_toolbar_, map_bag_gui_);
    const auto portal_rect =
        game_tiled_map_->GetPortalRect(portal.GetOppositePorta());

    // Calculate the center of the rectangle.
    const float center_x =
        portal_rect.origin.x + (portal_rect.size.width / 2.0F);
    const float center_y =
        portal_rect.origin.y + (portal_rect.size.height / 2.0F);

    // Set the player's position to the center of the rectangle.
    game_tiled_map_->SetTeleportStatus(true);
    game_tiled_map_->SetPlayerPos(cocos2d::Vec2(center_x, center_y));
}

void MapController::SaveTiledMap(TiledMap* tiled_map) {}

MapController& MapController::GetInstance() {
    static MapController instance;
    return instance;
}

}  // namespace th_valley
