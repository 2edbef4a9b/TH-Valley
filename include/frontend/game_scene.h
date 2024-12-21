#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include "cocos2d.h"
#include "game/tiled_map.h"

namespace th_valley {

class GameScene : public cocos2d::Scene {
public:
    GameScene() = default;
    ~GameScene() override = default;
    GameScene(const GameScene& other) = delete;
    GameScene& operator=(const GameScene& other) = delete;
    GameScene(GameScene&& other) = delete;
    GameScene& operator=(GameScene&& other) = delete;

    bool init() override;

    CREATE_FUNC(GameScene);

private:
    static constexpr std::string_view kInitialMap = "Farm";
};

}  // namespace th_valley

#endif  // GAME_SCENE_H_
