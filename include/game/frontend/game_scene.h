#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include "cocos2d.h"

// 引入新路径下的头文件
#include "game/frontend/bag_gui.h"
#include "game/frontend/tool_bar.h"
#include "game/environment/tiled_map.h"
#include "game/items/bag.h"

namespace th_valley {

class GameScene : public cocos2d::Scene {
public:
    GameScene() = default;
    ~GameScene() override = default;
    
    bool init() override;

    // 获取工具栏实例 (供其他层使用)
    ToolBar* GetToolBar() { return tool_bar_; }
    BagGUI* GetBagGUI() { return bag_gui_; }

    CREATE_FUNC(GameScene);

private:
    void showBasicInfomation(); // 显示 FPS 等

    static constexpr std::string_view kInitialMap = "Farm";
    
    // UI 层
    BagGUI* bag_gui_ = nullptr;
    ToolBar* tool_bar_ = nullptr;
    
    // 数据层 (Scene 持有 Bag 实例，并分发给 GUI)
    Bag* player_bag_ = nullptr;
};

}  // namespace th_valley

#endif // GAME_SCENE_H_