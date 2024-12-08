#include "sample/sample_scene.h"
#include <cstddef>
#include <string>

bool SampleScene::init() {
    // Initialize super class first.
    if (!cocos2d::Scene::init()) {
        return false;
    }

    // Change the default resource root path from "Resources/" to "assets/".
    SetResourcePath("assets");

    // 创建精灵帧
    cocos2d::Vector<cocos2d::SpriteFrame*> animFrames;

    // 假设每个角色图像的宽高为 32*48，且在 spritesheet 中有 4 帧
    for (int i = 0; i < 4; i++) {
        // 计算每一帧的 Rect，假设它们在 spritesheet 中是横向排列的
        cocos2d::Rect frameRect(i * 32, 0, 32, 48);  // Y坐标应该是0
        auto frame =
            cocos2d::SpriteFrame::create("avatar/character.png", frameRect);
        if (frame) {
            animFrames.pushBack(frame);
        }
    }

    // 创建动画
    auto animation = cocos2d::Animation::createWithSpriteFrames(
        animFrames, 0.1f);  // 0.1秒每帧
    auto animate = cocos2d::Animate::create(animation);

    // 创建精灵并添加到场景
    auto character = cocos2d::Sprite::create(
        "avatar/character.png", cocos2d::Rect(0, 0, 32, 48));  // 初始显示第一帧
    character->setPosition(cocos2d::Vec2(240, 160));  // 设置初始位置
    this->addChild(character);
    // 运行动画
    character->runAction(cocos2d::RepeatForever::create(animate));
    /*// 添加触摸事件
        auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->onTouchBegan = [character](cocos2d::Touch* touch, cocos2d::Event* event) {
            // 点击屏幕时移动角色
            cocos2d::Vec2 touchLocation = touch->getLocation();
            character->runAction(cocos2d::MoveTo::create(1.0f, touchLocation));  // 1秒移动到点击位置
            return true;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);*/  
        // 创建键盘监听器
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();

    // 定义按键按下时的回调
    keyboardListener->onKeyPressed =
        [character](cocos2d::EventKeyboard::KeyCode keyCode,
                 cocos2d::Event *event) {
            cocos2d::Vec2 position =character->getPosition();  // 获取精灵当前的位置

        switch (keyCode) {
                case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                position.x -= 10;  // 向左移动
                break;
                case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                position.x += 10;  // 向右移动
                break;
                case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
                position.y += 10;  // 向上移动
                break;
                case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                position.y -= 10;  // 向下移动
                break;
            default:
                break;
        }

        // 更新精灵的位置
            character->setPosition(position);
    };

    // 添加键盘监听器到事件分发器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
                                                             character);
    return true;  // 确保返回 true，表示初始化成功
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
