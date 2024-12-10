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

    // 定义方向枚举
    enum direction { Down = 0, Right = 1, Up = 2, Left = 3 };

    bool init() override;
    void move();
    void handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                          cocos2d::Event* event,auto character);
    static void SetResourcePath(const std::string& path);
    CREATE_FUNC(avatarScene);
};

#endif  // AVATAR_SCENE_H_
