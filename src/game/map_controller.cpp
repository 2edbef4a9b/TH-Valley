#include "game/map_controller.h"

#include <string_view>

#include "game/NPC.h"
#include "game/tiled_map.h"
#include "utility/logger.h"

namespace th_valley {

void MapController::LoadTiledMap(const std::string& tiled_map,
                                 cocos2d::Node* parent) {
    game_tiled_map_ =
        TiledMap::create(kTiledMapPathPrefix + tiled_map + kTiledMapPathSuffix);
    game_tiled_map_->Load(tiled_map + ".txt");
    game_tiled_map_->SetMapName(tiled_map);
    if (game_tiled_map_ == nullptr) {
        Logger::GetInstance().LogError("Failed to load TiledMap: {}",
                                       tiled_map);
        return;
    }
    Logger::GetInstance().LogInfo("Loaded TiledMap: {}", tiled_map);

    parent->addChild(game_tiled_map_, 1);
    if (tiled_map == "Ranch") {
        game_tiled_map_->SpawnAnimal(5);
    }
    if (tiled_map == "Town") {
        Sandy* sandy = new Sandy;
        Abigail* abigail = new Abigail;
        Alex* alex = new Alex;
        Elliott* elliott = new Elliott;
        Evelyn* evelyn = new Evelyn;
        game_tiled_map_->SpawnCitizen(sandy);
        game_tiled_map_->SpawnCitizen(abigail);
        game_tiled_map_->SpawnCitizen(alex);
        game_tiled_map_->SpawnCitizen(elliott);
        game_tiled_map_->SpawnCitizen(evelyn);
    }
    if (tiled_map == "Shop") {
        Sebastian* sebastian = new Sebastian;
        game_tiled_map_->SpawnCitizen(sebastian);
    }
    game_tiled_map_->initCitizenPosition();
    LoadRain(tiled_map);
}

void MapController::LoadRain(const std::string& tiled_map) {
    auto* rain = cocos2d::ParticleRain::create();
    rain->setPosition(cocos2d::Vec2(
        game_tiled_map_->getTiledMap()->getContentSize().width / 2,
        game_tiled_map_->getTiledMap()->getContentSize().height));
    rain->setPosVar(cocos2d::Vec2(
        game_tiled_map_->getTiledMap()->getContentSize().width / 2,
        0));  // 雨范围覆盖整个地图
    // rain->setLife(3.0f);          // 调整雨滴存活时间
    // rain->setLifeVar(1.0f);       // 存活时间的随机范围
    rain->setSpeed(300.0f);       // 设置雨滴下落速度
    rain->setSpeedVar(50.0f);     // 雨滴速度随机变化范围
    rain->setStartSize(5.0f);     // 雨滴大小
    rain->setStartSizeVar(2.0f);  // 雨滴大小随机范围

    rain->setVisible(false);
    game_tiled_map_->getTiledMap()->addChild(rain, 199);  // 添加到场景
    game_tiled_map_->getTiledMap()->schedule(
        [rain, tiled_map](float dt) {
            // 如果状态改变，则处理粒子效果
            if (GlobalWeather.WeatherType != "Rainy" || (tiled_map == "Barn") ||
                (tiled_map == "Cave") || (tiled_map == "FarmHouse") ||
                (tiled_map == "House") || (tiled_map == "Shop") ||
                (tiled_map == "NPC4") || (tiled_map == "NPC3") ||
                (tiled_map == "NPC2") || (tiled_map == "NPC1") ||
                (tiled_map == "Bar")) {
                rain->setVisible(false);
            } else {
                rain->setVisible(true);
            }
        },
        1.0f, "CheckRainCondition");  // 每秒检查一次变量值
}

void MapController::TriggerTeleport(const std::string& portal_name) {
    const TiledMap::Portal portal(portal_name);

    // Check if the map file exists.
    std::string map_path =
        kTiledMapPathPrefix + portal.GetToMap() + kTiledMapPathSuffix;
    if (!cocos2d::FileUtils::getInstance()->isFileExist(map_path)) {
        Logger::GetInstance().LogError("Map file not found: {}", map_path);
        return;
    }

    // Save and remove the current map.
    SaveTiledMap(game_tiled_map_, portal.GetFromMap());
    auto* parent = game_tiled_map_->getParent();
    if (parent == nullptr) {
        Logger::GetInstance().LogError("Parent node is null");
        return;
    }
    game_tiled_map_->unscheduleUpdate();
    parent->removeChild(game_tiled_map_);

    // Load the new map and set the player's position.
    LoadTiledMap(portal.GetToMap(), parent);
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

void MapController::SaveTiledMap(TiledMap* tiled_map,
                                 const std::string& map_name) {
    if (tiled_map == nullptr) {
        Logger::GetInstance().LogError("TiledMap is null");
        return;
    }

    const std::string save_path = map_name + ".txt";
    tiled_map->Save(save_path);
    Logger::GetInstance().LogInfo("Saved TiledMap: {}", tiled_map->getName());
}

MapController& MapController::GetInstance() {
    static MapController instance;
    return instance;
}

}  // namespace th_valley
