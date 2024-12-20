#include "frontend/game_scene.h"

#include "utility/logger.h"

namespace th_valley {

bool GameScene::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }

    game_tiled_map_ = TiledMap::create("assets/maps/Farm.tmx");
    if (game_tiled_map_) {
        auto visible_size = cocos2d::Director::getInstance()->getVisibleSize();
        auto map_size = game_tiled_map_->getContentSize();

        game_tiled_map_->setVisible(true);
        this->addChild(game_tiled_map_, 0);
    } else {
        Logger::GetInstance().LogError("Failed to load map maps/Farm.tmx");
    }

    return true;
}

}  // namespace th_valley
