#include "sample/sample_scene.h"
#include <cstddef>
#include <string>
#include "talk_box.h"
#include "tool_bar.h"
using namespace cocos2d;



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

        _map->setPosition(cocos2d::Vec2(0, 0));
        _map->setVisible(true);
        this->addChild(_map);
    } else
        CCLOG("Failed to load map maps/Farm.tmx");

    Sprite* _buttonSprite = Sprite::create("assets/Portraits/Abigail.png");
    _buttonSprite->setPosition(300,200);

    this->addChild(_buttonSprite,50);
    // 创建鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();

    int count = 0;
     mouseListener->onMouseMove = [=](EventMouse* event) {
        Vec2 mousePos = event->getLocation();
    };
    mouseListener->onMouseDown = [=](EventMouse* event) mutable{
        Vec2 mousePos = event->getLocation();
        if (this->getBoundingBox().containsPoint(mousePos)) {
            CCLOG("Button pressed!");
            if (this->getBoundingBox().containsPoint(mousePos)) {
                // 鼠标在按钮上，改变颜色
                if (count%2==0) {
                     _buttonSprite->setColor(
                         Color3B(255, 255, 0));  // 设置为黄色高亮
                } else {
                    _buttonSprite->setColor(
                        Color3B(156, 173, 0));  // 设置为黄色高亮
                }
                count++;
             }
         }
    };



    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener,
                                                             this);
    return true;
}

}  // namespace th_valley
