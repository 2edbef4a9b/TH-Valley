#include "sample/sample_scene.h"
#include <cstddef>
#include <string>

bool SampleScene::init()
{
    // ��ʼ������
    if (!cocos2d::Scene::init())
    {
        return false;
    }

    // ����Ĭ����Դ·��
    SetResourcePath("assets");

    // ���巽��ö��
    enum direction { Down = 0, Right = 1, Up = 2, Left = 3};

    // �������鲢��ӵ�����
    auto character = cocos2d::Sprite::create("avatar/sandy.png");
    character->setTextureRect(cocos2d::Rect(0, 0, 16, 32));  // ��ʼ��ʾ��һ֡
    character->setPosition(cocos2d::Vec2(240, 160));  // ���ó�ʼλ��
    this->addChild(character);

    // �������̼�����
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    // ���尴������ʱ�Ļص�
    keyboardListener->onKeyPressed =
        [character](cocos2d::EventKeyboard::KeyCode keyCode,
                    cocos2d::Event *event) {
            cocos2d::Vec2 position =
                character->getPosition();  // ��ȡ���鵱ǰ��λ��
            direction dir = Down;
            // ���·����λ��
            switch (keyCode) {
                case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                    dir = Left;
                    position.x -= 10;  // �����ƶ�
                    break;
                case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                    dir = Right;
                    position.x += 10;  // �����ƶ�
                    break;
                case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
                    dir = Up;
                    position.y += 10;  // �����ƶ�
                    break;
                case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                    dir = Down;
                    position.y -= 10;  // �����ƶ�
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
                animFrames, 0.1f);  // 0.1��ÿ֡
            auto animate = cocos2d::Animate::create(animation);
            // ���ж���
            // ֹ֮ͣǰ�����ж���
            character->stopAllActions();
            character->runAction(cocos2d::RepeatForever::create(animate));

            animFrames.clear();
            return true;
    };

    // ��Ӽ��̼��������¼��ַ���
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,character);
        
    

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
