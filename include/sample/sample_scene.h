#ifndef SAMPLE_SCENE_H_
#define SAMPLE_SCENE_H_

#include "avatar/avatar.h"
#include "bag.h"
#include "bag_gui.h"
#include "cocos2d.h"
#include "tool_bar.h"

enum direction { Down = 0, Right = 1, Up = 2, Left = 3 };

namespace th_valley {

class SampleScene final : public cocos2d::Scene {
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

    CREATE_FUNC(SampleScene);
};

}  // namespace th_valley

#endif  // SAMPLE_SCENE_H_
