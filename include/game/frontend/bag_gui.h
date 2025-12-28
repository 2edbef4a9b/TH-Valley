#ifndef BAG_GUI_H_
#define BAG_GUI_H_

#include <string>
#include <string_view>
#include <map>
#include <vector>

#include "cocos2d.h"
#include "game/items/bag.h"
#include "game/items/item_system.h" // 引入 ItemStack
// #include "game/frontend/tool_bar.h" // 避免循环引用，使用前向声明

class ToolBar; 

class BagGUI : public cocos2d::Layer {
public:
    BagGUI() = default;
    ~BagGUI() override = default;

    // 基础 Cocos 方法
    bool init() override;
    void onEnter() override;
    CREATE_FUNC(BagGUI);

    // 绑定数据
    void SetBag(Bag* bag) { bag_ = bag; }
    void SetToolBar(ToolBar* tb) { toolBar = tb; }

    void toggleBag();  // 开/关背包

private:
    static constexpr std::string_view kFontPath = "assets/fonts/DFHannotateW5-A.ttf";

    Bag* bag_ = nullptr;
    ToolBar* toolBar = nullptr;

    cocos2d::Size visible_size_;
    cocos2d::Vec2 visible_origin_;
    
    // UI 组件
    cocos2d::Sprite* background_;
    cocos2d::MenuItemImage* closeButton_;
    cocos2d::Menu* closeButtonMenu_;
    
    // 物品显示
    // 使用 ItemStack* 而不是旧的 ItemSprite*
    std::map<int, cocos2d::Sprite*> itemIcons_; 
    std::map<int, cocos2d::Label*> itemCounts_;

    bool isBagOpen = false;
    
    // 交互逻辑
    cocos2d::Vec2 originalPosition;
    int draggingSlotIndex = -1;       // 改为记录 Slot Index 而不是 ItemIndex
    cocos2d::Sprite* selectedItemSprite = nullptr;

    void initBagGUI();
    void createCloseButton();
    
    void displayBagItems();
    void hideBagItems();
    void refreshView(); // 新增：刷新整个界面

    // 输入事件
    void onMouseDown(cocos2d::Event* event);
    void onMouseMove(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);
};

#endif  // BAG_GUI_H_