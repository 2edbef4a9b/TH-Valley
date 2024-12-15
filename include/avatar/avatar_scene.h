#ifndef AVATAR_SCENE_H_
#define AVATAR_SCENE_H_
#include"avatar/avatar.h"
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
    // 定义方向枚举
    enum direction { Down = 0, Right = 1, Up = 2, Left = 3 };
    float idleTime = 0.0f;             // 记录无输入时间
    const float idleThreshold = 0.5f;  // 设定无输入的阈值
    bool moved = false;                // 记录是否发生移动
    bool init() override;
    void keyboardreading();
    cocos2d::Sprite *character;

    Avatar haley;

    direction dir;
    const float speed = 10.0f;
    cocos2d::Vec2 handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                          cocos2d::Event* event, cocos2d::Vec2 position);
    void handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                           cocos2d::Event* event);
    static void SetResourcePath(const std::string& path);

    void animateRunning();

    CREATE_FUNC(avatarScene);
};

#endif  // AVATAR_SCENE_H_
