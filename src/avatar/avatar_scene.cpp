#include <cstddef>
#include <string>
#include "avatar/avatar_scene.h"

bool avatarScene::init() {
    // ��ʼ������
    if (!cocos2d::Scene::init()) {
        return false;
    }
    move();
    return true;  // ȷ������ true����ʾ��ʼ���ɹ�
}
void avatarScene::handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                                   cocos2d::Event *event,
                                   auto character) {
    cocos2d::Vec2 position = character->getPosition();  // ��ȡ���鵱ǰ��λ��
    direction dir= Down;
    const float speed=10.0f;
    // ���·����λ��
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
    // ���¶���֡
    cocos2d::Vector<cocos2d::SpriteFrame *> animFrames;
    for (int i = 0; i < 4; i++) {
        cocos2d::Rect frameRect(i * 16, dir * 32, 16, 32);
        auto frame =
            cocos2d::SpriteFrame::create("avatar/sandy.png", frameRect);
        if (frame) {
            animFrames.pushBack(frame);
        }
    }
    // ���¾����λ��
    character->setPosition(position);
    // ��������
    auto animation = cocos2d::Animation::createWithSpriteFrames(
        animFrames, float(0.1)); 
    auto animate = cocos2d::Animate::create(animation);
    // ���ж���
    // ֹ֮ͣǰ�����ж���
    character->stopAllActions();
    character->runAction(cocos2d::RepeatForever::create(animate));
    animFrames.clear();
}
void avatarScene::move() {
    // ����Ĭ����Դ·��
    SetResourcePath("assets");
    // �������鲢��ӵ�����
    auto character = cocos2d::Sprite::create("avatar/sandy.png");
    character->setTextureRect(cocos2d::Rect(0, 0, 16, 32));  // ��ʼ��ʾ��һ֡
    character->setPosition(cocos2d::Vec2(240, 160));  // ���ó�ʼλ��
    this->addChild(character);
    // �������̼�����
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    // ���尴������ʱ�Ļص�
    keyboardListener->onKeyPressed =
        [this,character](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
            this->handleKeyPressed(keyCode, event,character);
    };
    // ��Ӽ��̼��������¼��ַ���
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
