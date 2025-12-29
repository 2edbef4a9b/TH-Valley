#ifndef BAG_GUI_H_
#define BAG_GUI_H_

#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "cocos2d.h"
#include "game/items/bag.h"
#include "game/items/item_system.h"  // Introduce ItemStack
// #include "game/frontend/tool_bar.h" // Avoid circular reference, use forward
// declaration

class ToolBar;

class BagGUI : public cocos2d::Layer {
public:
    BagGUI() = default;
    ~BagGUI() override = default;

    // Basic Cocos methods
    bool init() override;
    void onEnter() override;
    CREATE_FUNC(BagGUI);

    // Bind data
    void SetBag(Bag* bag) { bag_ = bag; }
    void SetToolBar(ToolBar* tb) { toolBar = tb; }

    void toggleBag();  // Open/close backpack

private:
    static constexpr std::string_view kFontPath =
        "assets/fonts/DFHannotateW5-A.ttf";

    Bag* bag_ = nullptr;
    ToolBar* toolBar = nullptr;

    cocos2d::Size visible_size_;
    cocos2d::Vec2 visible_origin_;

    // UI Components
    cocos2d::Sprite* background_;
    cocos2d::MenuItemImage* closeButton_;
    cocos2d::Menu* closeButtonMenu_;

    // Item display
    // Use ItemStack* instead of old ItemSprite*
    std::map<int, cocos2d::Sprite*> itemIcons_;
    std::map<int, cocos2d::Label*> itemCounts_;

    bool isBagOpen = false;

    // Interaction logic
    cocos2d::Vec2 originalPosition;
    int draggingSlotIndex =
        -1;  // Changed to record Slot Index instead of ItemIndex
    cocos2d::Sprite* selectedItemSprite = nullptr;

    void initBagGUI();
    void createCloseButton();

    void displayBagItems();
    void hideBagItems();
    void refreshView();  // Added: refresh entire interface

    // Input events
    void onMouseDown(cocos2d::Event* event);
    void onMouseMove(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);
};

#endif  // BAG_GUI_H_
