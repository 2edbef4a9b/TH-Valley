#ifndef SAMPLE_SCENE_H_
#define SAMPLE_SCENE_H_

#include "avatar/avatar.h"
#include "cocos2d.h"
#include "frontend/bag_gui.h"
#include "frontend/tool_bar.h"
#include "game/bag.h"

enum direction { Down = 0, Right = 1, Up = 2, Left = 3 };

namespace th_valley {

class SampleScene : public cocos2d::Scene {
public:
    ToolBar* CurrentToolBar;
    BagGUI* CurrentBag;

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
    std::vector<direction> Alldir;
    bool moved = false;
    bool isattack = false;  // ¼ÇÂ¼ÊÇ·ñ·¢ÉúÒÆ¶¯
    direction dir;
    bool isUp = false;
    bool isDown = false;
    bool isLeft = false;
    bool isRight = false;

    cocos2d::TMXTiledMap* map_;

    void keyboardreading();
    void handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                          cocos2d::Event* event);
    void handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                           cocos2d::Event* event);

    static void SetResourcePath(const std::string& path);

    CREATE_FUNC(SampleScene);
};

}  // namespace th_valley

#endif  // SAMPLE_SCENE_H_
