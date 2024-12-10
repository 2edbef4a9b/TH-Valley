#include <cstddef>
#include <string>
#include "avatar/avatar_scene.h"

bool avatarScene::init() {
    // 初始化父类
    if (!cocos2d::Scene::init()) {
        return false;
    }
    move();
    return true;  // 确保返回 true，表示初始化成功
}
void avatarScene::handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                                   cocos2d::Event *event,
                                   auto character) {
    cocos2d::Vec2 position = character->getPosition();  // 获取精灵当前的位置
    direction dir= Down;
    const float speed=10.0f;
    // 更新方向和位置
    switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            dir = Left;
            position.x -=speed;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            dir = Right;
            position.x += speed;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
            dir = Up;
            position.y +=speed;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            dir = Down;
            position.y -= speed;
            break;
        default:
            break;
    }
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
    // 更新精灵的位置
    character->setPosition(position);
    // 创建动画
    auto animation = cocos2d::Animation::createWithSpriteFrames(
        animFrames, float(0.1)); 
    auto animate = cocos2d::Animate::create(animation);
    // 运行动画
    // 停止之前的所有动作
    character->stopAllActions();
    character->runAction(cocos2d::RepeatForever::create(animate));
    animFrames.clear();
}
void avatarScene::move() {
    // 更改默认资源路径
    SetResourcePath("assets");
    // 创建精灵并添加到场景
    auto character = cocos2d::Sprite::create("avatar/sandy.png");
    character->setTextureRect(cocos2d::Rect(0, 0, 16, 32));  // 初始显示第一帧
    character->setPosition(cocos2d::Vec2(240, 160));  // 设置初始位置
    this->addChild(character);
    // 创建键盘监听器
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    // 定义按键按下时的回调
    keyboardListener->onKeyPressed =
        [this,character](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
            this->handleKeyPressed(keyCode, event,character);
    };
    // 添加键盘监听器到事件分发器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
                                                             character);
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
