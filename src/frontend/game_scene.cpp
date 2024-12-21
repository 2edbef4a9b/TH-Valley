#include "frontend/game_scene.h"

#include "game/map_controller.h"
#include "utility/logger.h"

namespace th_valley {

bool GameScene::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }

    MapController::GetInstance().LoadTiledMap(kInitialMap.data(), this);

    return true;
}

}  // namespace th_valley
