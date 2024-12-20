#ifndef AVATAR_SCENE_H_
#define AVATAR_SCENE_H_
#include"avatar/avatar.h"
#include "cocos2d.h"


enum direction { Down = 0, Right = 1, Up = 2, Left = 3 };


class avatarScene : public cocos2d::Scene {
public:
    // operation
    avatarScene() = default;
    ~avatarScene() override = default;
    avatarScene(const avatarScene& other) = default;
    avatarScene& operator=(const avatarScene& other) = default;
    avatarScene(avatarScene&& other) = default;
    avatarScene& operator=(avatarScene&& other) = default;

    // action animate running
    // walking;
    void Moving(int dir, float flu);
    // when upgrade;
    void upgrading();
    // do daily work
    void Working(std::string action);
    // launch attack
    void Attacking(std::string weaponTypes);
    // avatar
    cocos2d::Sprite* Haley;
    // directions
    direction dir;
    // control mark
    bool moved = false;
    bool isattack = false;  
    bool isfish = false;




    
    // map test
    cocos2d::TMXTiledMap* map_;
    // cocos members
    void keyboardreading();
    void handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                          cocos2d::Event* event);
    void handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                           cocos2d::Event* event);
    void update(float dt);
    bool init() override;

    CREATE_FUNC(avatarScene);

    // should be delete
    static void SetResourcePath(const std::string& path);
};

#endif  // AVATAR_SCENE_H_
