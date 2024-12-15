#include "sample/sample_scene.h"

#include <cstddef>
#include <string>

#include <vector>

#include "map.h"

#include <vector>

#include "map.h"

bool th_valley::SampleScene::init() {
    // Initialize super class first.
    if (!cocos2d::Scene::init()) {
        return false;
    }

    // Create a label and add it to the scene.
    /*constexpr size_t kFontSize = 120;
    auto *label = cocos2d::Label::createWithTTF(
        "Hello World!~", "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
    label->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2));

    this->addChild(label);*/

    //const auto _map = cocos2d::FastTMXTiledMap::create("maps/Farm.tmx");
    //if (_map)
    //    CCLOG("mapLayer found");

    //_map->setPosition(cocos2d::Vec2(0, 0));
    //_map->setVisible(true);

    //this->addChild(_map);
    
    auto _map = Map::create("maps/Farm.tmx");
    if (_map) {
        _map->setPosition(cocos2d::Vec2(0, 0));
        _map->setVisible(true);
        this->addChild(_map);
    } else
        CCLOG("Failed to load map maps/Farm.tmx");
    
    return true;
}
