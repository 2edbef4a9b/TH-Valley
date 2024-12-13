#include "sample/sample_scene.h"

#include <cstddef>
#include <string>

#include <vector>

#include "map.h"

bool SampleScene::init() {
    // Initialize super class first.
    if (!cocos2d::Scene::init()) {
        return false;
    }

    // Change the default resource root path from "Resources/" to "assets/".
    SetResourcePath("assets");

    // Create a label and add it to the scene.
    /*constexpr size_t kFontSize = 120;
    auto *label = cocos2d::Label::createWithTTF(
        "Hello World!~", "fonts/DFHannotateW5-A.ttf", kFontSize);
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
    void SampleScene::SetResourcePath(const std::string &path) {
        auto *file_utils = cocos2d::FileUtils::getInstance();

        // Get the current resource root absolute path.
        std::string resource_root_path =
            cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();

        // Replace the last directory name with the new path.
        const size_t pos =
            resource_root_path.find_last_of('/', resource_root_path.size() - 2);
        if (pos != std::string::npos) {
            resource_root_path.replace(pos + 1, std::string::npos, path + "/");
        } else {
            resource_root_path = path + "/";
        }

        // Update the default resource root path.
        file_utils->setDefaultResourceRootPath(resource_root_path);
    }
