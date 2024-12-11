#ifndef AVATAR_SCENE_H_
#define AVATAR_SCENE_H_

#include "cocos2d.h"

class avatarScene : public cocos2d::Scene {
public:
    avatarScene() = default;
    ~avatarScene() override = default;
    avatarScene(const avatarScene& other) = default;
    avatarScene& operator=(const avatarScene& other) = default;
    avatarScene(avatarScene&& other) = default;
    avatarScene& operator=(avatarScene&& other) = default;
    cocos2d::TMXTiledMap* map_;
    // ���巽��ö��
    enum direction { Down = 0, Right = 1, Up = 2, Left = 3 };
    float idleTime = 0.0f;             // ��¼������ʱ��
    const float idleThreshold = 0.5f;  // �趨���������ֵ
    bool moved = false;                // ��¼�Ƿ����ƶ�
    bool init() override;
    void move(auto &character);
    int _movetimes = 0;
    void handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                          cocos2d::Event* event, auto& character);
    void handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                                        cocos2d::Event* event);
    static void SetResourcePath(const std::string& path);
    CREATE_FUNC(avatarScene);
};

#endif  // AVATAR_SCENE_H_
