#include <cstddef>
#include <string>
#include "avatar/avatar_scene.h"

bool avatarScene::init() {
    // 初始化父类
    if (!cocos2d::Scene::init()) {
        return false;
    }
    // 更改默认资源路径
    SetResourcePath("assets");
    const auto _map = cocos2d::TMXTiledMap::create("maps/Farm.tmx");
    if (_map) CCLOG("mapLayer found");

    _map->setPosition(cocos2d::Vec2(0, 0));
    _map->setVisible(true);
    this->addChild(_map);


    // 创建精灵并添加到场景
    auto character = cocos2d::Sprite::create("avatar/sandy.png");
    character->setTextureRect(cocos2d::Rect(0, 0, 16, 32));  // 初始显示第一帧
    character->setPosition(cocos2d::Vec2(240, 160));  // 设置初始位置
    this->addChild(character);
    move(character);
    
    return true;  // 确保返回 true，表示初始化成功
}
void avatarScene::handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                                   cocos2d::Event *event){
    moved = false;
}
void avatarScene::handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                                   cocos2d::Event *event, auto &character) {
    cocos2d::Vec2 position = character->getPosition();  // 获取精灵当前的位置
    direction dir = Down;
    const float speed = 10.0f;
    // 更新方向和位置
    switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            dir = Left;
            position.x -= speed;
            moved = true;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            dir = Right;
            position.x += speed;
            moved = true;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            dir = Up;
            position.y += speed;
            moved = true;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            dir = Down;
            position.y -= speed;
            moved = true;
            break;
        default:
            moved = false;  // 如果按下的不是移动键，标记为未移动
            break;
    }
    // 只有在角色移动时才更新位置和动画
    //     // 更新精灵的位置
    character->setPosition(position);
    // 更新动画帧
    cocos2d::Vector<cocos2d::SpriteFrame *> animFrames;
    for (int i = 0; i < 4; i++) {
        cocos2d::Rect frameRect(i * 16, dir * 32, 16, 32);
        auto frame =
            cocos2d::SpriteFrame::create("avatar/sandy.png", frameRect);
        if (frame) {
            animFrames.pushBack(frame);
        }
    }

    // 创建动画
    auto animation =
        cocos2d::Animation::createWithSpriteFrames(animFrames, float(0.1));
    auto animate = cocos2d::Animate::create(animation);

    // 停止之前的所有动作
    character->stopAllActions();
    character->runAction(cocos2d::RepeatForever::create(animate));
    // 重置无输入时间
    idleTime = 0.0f;

   


    
    
}

void avatarScene::move(auto &character) {
    // 更改默认资源路径
    SetResourcePath("assets");
    // 创建键盘监听器
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    /**/
    // 定义按键按下时的回调
    keyboardListener->onKeyPressed =
        [this, character](cocos2d::EventKeyboard::KeyCode keyCode,
                          cocos2d::Event *event) {
            this->handleKeyPressed(keyCode, event, character);
        };
    keyboardListener->onKeyReleased =
        [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
            this->handleKeyReleased(keyCode, event);
        };

    // 添加键盘监听器到事件分发器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,character);
    schedule(
        [this, character](float dt) {
            // 每帧更新无输入时间
            if (!moved) {
                idleTime += dt;
                if (idleTime >= idleThreshold) {
                    // 停止动画并设置为静止帧
                    character->stopAllActions();
                    character->setTextureRect(
                        cocos2d::Rect(0, 0, 16, 32));  // 设置为静止帧
                }
            } else {
                idleTime = 0.0f;  // 重置无输入时间

                _movetimes++;
            }
        },
        "idle_update_key");
}

void avatarScene::SetResourcePath(const std::string &path) {
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
