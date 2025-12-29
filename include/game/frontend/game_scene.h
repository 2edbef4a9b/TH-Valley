#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include "cocos2d.h"

// Include header files from new path
#include "game/environment/tiled_map.h"
#include "game/frontend/bag_gui.h"
#include "game/frontend/tool_bar.h"
#include "game/items/bag.h"

namespace th_valley {

class GameScene : public cocos2d::Scene {
public:
    GameScene() = default;
    ~GameScene() override = default;

    bool init() override;

    // Get toolbar instance (for use by other layers)
    ToolBar* GetToolBar() { return tool_bar_; }
    BagGUI* GetBagGUI() { return bag_gui_; }

    CREATE_FUNC(GameScene);

private:
    void showBasicInfomation();  // Display FPS etc.

    static constexpr std::string_view kInitialMap = "Farm";

    // UI Layer
    BagGUI* bag_gui_ = nullptr;
    ToolBar* tool_bar_ = nullptr;

    // Data Layer (Scene holds Bag instance and distributes to GUI)
    Bag* player_bag_ = nullptr;
};

}  // namespace th_valley

#endif  // GAME_SCENE_H_
