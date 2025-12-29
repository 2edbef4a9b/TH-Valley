#ifndef TOOL_BAR_H_
#define TOOL_BAR_H_

#include <string>
#include <vector>

#include "cocos2d.h"
#include "game/items/bag.h"

class ToolBar : public cocos2d::Layer {
public:
    ToolBar() = default;
    ~ToolBar() override = default;
    bool init() override;
    void onEnter() override;
    CREATE_FUNC(ToolBar);

    // Bind backpack data
    void SetBag(Bag* bag) { bag_ = bag; }

    // Core functions
    void selectTool(int index);
    void updateToolBar();  // Read first 10 slots of Bag

    std::string getToolName();
    int getToolIndex() { return selectedToolIndex; }

private:
    Bag* bag_ = nullptr;
    int selectedToolIndex = 0;

    static constexpr std::string_view kFontPath =
        "assets/fonts/DFHannotateW5-A.ttf";

    cocos2d::Size visible_size_;
    cocos2d::Vec2 visible_origin_;

    // UI Components
    cocos2d::Sprite* background_;
    std::vector<cocos2d::LayerColor*> toolBorders;
    std::vector<cocos2d::Label*> toolCounts;
    cocos2d::Sprite* toolIcons[10] = {nullptr};
    cocos2d::Label* MoneyLabel;

    // Drag related
    cocos2d::Vec2 draggingToolOffset;
    int draggingToolIndex = -1;

    void addToolLable();       // Label spelling correction
    void updateToolDisplay();  // Refresh icon display
    void dropCurrentTool();    // Drop logic
};

#endif  // TOOL_BAR_H_
