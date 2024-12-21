#include "sample/sample_scene.h"

#include <cstddef>
#include <string>

#include "game/tiled_map.h"

namespace th_valley {

bool SampleScene::init() {
    // Initialize super class first.
    if (!cocos2d::Scene::init()) {
        return false;
    }

    auto _map = TiledMap::create("assets/maps/House.tmx");
    if (_map) {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        auto mapSize = _map->getContentSize();

        _map->setVisible(true);
        this->addChild(_map);
    } else
        CCLOG("Failed to load map maps/Farm.tmx");

    return true;
}

}  // namespace th_valley
