#include <cstddef>
#include <string>
#include "avatar/avatar_scene.h"

void avatarScene::animateRunning()
{
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
    // ��������
    auto animation =
        cocos2d::Animation::createWithSpriteFrames(animFrames, float(0.1));
    auto animate = cocos2d::Animate::create(animation);
    character->runAction(cocos2d::RepeatForever::create(animate));         
}
bool avatarScene::init() {
    // ��ʼ������
    if (!cocos2d::Scene::init()) {
        return false;
    }
    // ����Ĭ����Դ·��
    SetResourcePath("assets");



    const auto _map = cocos2d::TMXTiledMap::create("maps/Farm.tmx");
    if (_map) CCLOG("mapLayer found");
    _map->setPosition(cocos2d::Vec2(0, 0));
    _map->setVisible(true);
    this->addChild(_map);




    // �������鲢��ӵ�����
    character = cocos2d::Sprite::create("avatar/sandy.png");
    character->setTextureRect(cocos2d::Rect(0, 0, 16, 32));  // ��ʼ��ʾ��һ֡
    character->setPosition(cocos2d::Vec2(240, 160));  // ���ó�ʼλ��
    this->addChild(character);
    keyboardreading();
    return true;  // ȷ������ true����ʾ��ʼ���ɹ�
}
void avatarScene::handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                                    cocos2d::Event *event) {
    moved = false;
}
cocos2d::Vec2 avatarScene::handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                                   cocos2d::Event *event,
                                   cocos2d::Vec2 position) {
    // ���尴������ʱ�Ļص�
    double a = position.x;
    double b = position.y;
    // ���·����λ��
    switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            dir = Left;
            a -= speed;
            moved = true;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            dir = Right;
            a += speed;
            moved = true;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            dir = Up;
             b += speed;
            moved = true;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            dir = Down;
            b -= speed;
            moved = true;
            break;
        default:
            moved = false;  // ������µĲ����ƶ��������Ϊδ�ƶ�
            break;
    }
    animateRunning();
    return cocos2d::Vec2(a, b);
}

void avatarScene::keyboardreading() {
    // ����Ĭ����Դ·��
    SetResourcePath("assets");
    // �������̼�����
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    // ���尴������ʱ�Ļص�
    cocos2d::Vec2 position = character->getPosition();  // ��ȡ���鵱ǰ��λ��
    cocos2d::Vec2 positionout;
    keyboardListener->onKeyPressed =
        [=](cocos2d::EventKeyboard::KeyCode keyCode,cocos2d::Event *event) mutable{
            positionout.set(this->handleKeyPressed(keyCode, event, position).x,
                            this->handleKeyPressed(keyCode, event, position).y);
            CCLOG("=======");
            CCLOG("%lf,%lf", positionout.x, positionout.y);
            CCLOG("=======");

            character->setPosition(positionout);
            position =character->getPosition();  // ��ȡ���鵱ǰ��λ��
        };
    keyboardListener->onKeyReleased =
        [=](cocos2d::EventKeyboard::KeyCode keyCode,
            cocos2d::Event *event) mutable {
            this->handleKeyReleased(keyCode, event);
        };
    CCLOG("=======");
    // ��Ӽ��̼��������¼��ַ���
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,character);
    schedule(
        [this](float dt) {
            // ÿ֡����������ʱ��
            if (!moved) {
                idleTime += dt;
                if (idleTime >= idleThreshold) {
                    // ֹͣ����������Ϊ��ֹ֡
                    character->stopAllActions();
                    character->setTextureRect(
                        cocos2d::Rect(0, 0, 16, 32));  // ����Ϊ��ֹ֡
                }
            } else {
                idleTime = 0.0f;  // ����������ʱ��

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
