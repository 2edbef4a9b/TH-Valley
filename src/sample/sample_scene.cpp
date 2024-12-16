#include "sample/sample_scene.h"

#include <cstddef>
#include <string>

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

    
    auto _map = Map::create("assets/maps/Town.tmx");
    if (_map) {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        auto mapSize = _map->getContentSize();

        //// 设置地图的锚点为左下角
        //_map->setAnchorPoint(cocos2d::Vec2(0, 0));

        /*_map->setPosition(
            cocos2d::Vec2((visibleSize.width - mapSize.width) / 2,
                          (visibleSize.height - mapSize.height) / 2));*/

        _map->setVisible(true);
        this->addChild(_map);
    } else
        CCLOG("Failed to load map maps/Farm.tmx");
    
    return true;
}
