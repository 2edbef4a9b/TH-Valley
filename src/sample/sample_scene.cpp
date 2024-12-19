#include "sample/sample_scene.h"
#include <cstddef>
#include <string>
#include "avatar/avatar.h"

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

    Haley = cocos2d::Sprite::create("assets/avatar/Haley.png");
    Haley->setTextureRect(cocos2d::Rect(0, 0, 16, 32));  // Set initial frame to the first frame
    auto _map = Map::create("assets/maps/Farm.tmx", Haley);


    // Create sprite and add it to the scene
    //Haley->setPosition(cocos2d::Vec2(240, 160));  // Set initial position
    //this->addChild(Haley);
    //_map->playerSprite = Haley;

    if (_map) {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        auto mapSize = _map->getContentSize();

        //_map->setAnchorPoint(cocos2d::Vec2(0, 0));

        /*_map->setPosition(
            cocos2d::Vec2((visibleSize.width - mapSize.width) / 2,
                          (visibleSize.height - mapSize.height) / 2));*/

        _map->setVisible(true);
        this->addChild(_map);
        keyboardreading();  // Start reading keyboard input
    } else
        CCLOG("Failed to load map maps/Farm.tmx");

    return true;  // Ensure to return true to indicate successful initialization
}
