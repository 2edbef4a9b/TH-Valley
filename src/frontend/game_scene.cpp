#include "frontend/game_scene.h"

#include "frontend/bag_gui.h"
#include "frontend/tool_bar.h"
#include "game/map_controller.h"
#include "utility/logger.h"

namespace th_valley {

bool GameScene::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }
    tool_bar_ = ToolBar::create();
    bag_gui_ = BagGUI::create();
    tool_bar_->bag_ = bag_gui_->bag_;
    bag_gui_->toolBar = tool_bar_;
    this->addChild(tool_bar_, 21);
    this->addChild(bag_gui_, 20);

    MapController::GetInstance().LoadTiledMap(kInitialMap.data(), this);

    return true;
}

ToolBar* GameScene::GetToolBar() { return tool_bar_; }

}  // namespace th_valley
