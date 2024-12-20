#ifndef AVATAR_SCENE_H_
#define AVATAR_SCENE_H_
#include"avatar/avatar.h"
#include "cocos2d.h"


enum direction { Down = 0, Right = 1, Up = 2, Left = 3 };


class avatarScene : public cocos2d::Sprite {
public:
    avatarScene() = default;
    ~avatarScene() override = default;
    avatarScene(const avatarScene& other) = default;
    avatarScene& operator=(const avatarScene& other) = default;
    avatarScene(avatarScene&& other) = default;
    avatarScene& operator=(avatarScene&& other) = default;

    void animateRunning(int dir, float flu);
    //void cutting(cocos2d::Vec2 position);
    //void mining(cocos2d::Vec2 position);
    //void fishing(cocos2d::Vec2 position);
    //void cultivating(cocos2d::Vec2 position);
    //void attacking(cocos2d::Vec2 position);
    //void upgradeshow();
    void update(float dt);

    cocos2d::Sprite* Haley;
    bool moved = false;
    //bool isattack = false;  // 记录是否发生移动
    bool init() override;


    //void onEnter();
    //void handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
    //                      cocos2d::Event* event);
    //void handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
    //                       cocos2d::Event* event);

    //static void SetResourcePath(const/* */std::string& path);

    CREATE_FUNC(avatarScene);

    direction dir;

    

};

#endif  // AVATAR_SCENE_H_
