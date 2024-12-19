#ifndef SAMPLE_SCENE_H_
#define SAMPLE_SCENE_H_

#include "cocos2d.h"
#include "avatar/avatar.h"

enum direction { Down = 0, Right = 1, Up = 2, Left = 3 };

namespace th_valley {

class SampleScene : public cocos2d::Scene {
public:
    SampleScene() = default;
    ~SampleScene() override = default;
    SampleScene(const SampleScene& other) = delete;
    SampleScene& operator=(const SampleScene& other) = delete;
    SampleScene(SampleScene&& other) = delete;
    SampleScene& operator=(SampleScene&& other) = delete;

    bool init() override;

    // add declaration of tiledmap
    cocos2d::TMXTiledMap* _map;
    void animateRunning(int dir, float flu);
    void cutting(cocos2d::Vec2 position);
    void mining(cocos2d::Vec2 position);
    void fishing(cocos2d::Vec2 position);
    void cultivating(cocos2d::Vec2 position);
    void attacking(cocos2d::Vec2 position);
    void upgradeshow();
    void update(float dt);

    cocos2d::Sprite* Haley;
    bool moved = false;
    bool isattack = false;  // 记录是否发生移动

    cocos2d::TMXTiledMap* map_;

    void keyboardreading();
    void handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                          cocos2d::Event* event);
    void handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                           cocos2d::Event* event);

    static void SetResourcePath(const std::string& path);

    CREATE_FUNC(SampleScene);
    direction dir;
};

}  // namespace th_valley

#endif  // SAMPLE_SCENE_H_
