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

    // ��������֡
    cocos2d::Vector<cocos2d::SpriteFrame*> animFrames;

    // ����ÿ����ɫͼ��Ŀ��Ϊ 32*48������ spritesheet ���� 4 ֡
    for (int i = 0; i < 4; i++) {
        // ����ÿһ֡�� Rect������������ spritesheet ���Ǻ������е�
        cocos2d::Rect frameRect(i * 32, 0, 32, 48);  // Y����Ӧ����0
        auto frame =
            cocos2d::SpriteFrame::create("avatar/character.png", frameRect);
        if (frame) {
            animFrames.pushBack(frame);
        }
    }

    // ��������
    auto animation = cocos2d::Animation::createWithSpriteFrames(
        animFrames, 0.1f);  // 0.1��ÿ֡
    auto animate = cocos2d::Animate::create(animation);

    // �������鲢��ӵ�����
    auto character = cocos2d::Sprite::create(
        "avatar/character.png", cocos2d::Rect(0, 0, 32, 48));  // ��ʼ��ʾ��һ֡
    character->setPosition(cocos2d::Vec2(240, 160));  // ���ó�ʼλ��
    this->addChild(character);
    // ���ж���
    character->runAction(cocos2d::RepeatForever::create(animate));
    /*// ��Ӵ����¼�
        auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->onTouchBegan = [character](cocos2d::Touch* touch, cocos2d::Event* event) {
            // �����Ļʱ�ƶ���ɫ
            cocos2d::Vec2 touchLocation = touch->getLocation();
            character->runAction(cocos2d::MoveTo::create(1.0f, touchLocation));  // 1���ƶ������λ��
            return true;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);*/  
        // �������̼�����
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();

    // ���尴������ʱ�Ļص�
    keyboardListener->onKeyPressed =
        [character](cocos2d::EventKeyboard::KeyCode keyCode,
                 cocos2d::Event *event) {
            cocos2d::Vec2 position =character->getPosition();  // ��ȡ���鵱ǰ��λ��

        switch (keyCode) {
                case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                position.x -= 10;  // �����ƶ�
                break;
                case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                position.x += 10;  // �����ƶ�
                break;
                case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
                position.y += 10;  // �����ƶ�
                break;
                case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                position.y -= 10;  // �����ƶ�
                break;
            default:
                break;
        }

        // ���¾����λ��
            character->setPosition(position);
    };

    // ��Ӽ��̼��������¼��ַ���
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
                                                             character);
    return true;  // ȷ������ true����ʾ��ʼ���ɹ�
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
