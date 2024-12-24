#ifndef TOOL_BAR_H_
#define TOOL_BAR_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "cocos2d.h"
#include "game/bag.h"

class ToolBar : public cocos2d::Layer {
public:
    Bag* bag_;
    int selectedToolIndex;

    ToolBar() = default;
    ~ToolBar() override = default;
    ToolBar(const ToolBar& other) = delete;
    ToolBar& operator=(const ToolBar& other) = delete;
    ToolBar(ToolBar&& other) = delete;
    ToolBar& operator=(ToolBar&& other) = delete;
    bool init() override;
    void onEnter() override;

    CREATE_FUNC(ToolBar);

    void selectTool(int index);
    void updateToolBar();
    std::string getToolName();
    void loadTools();
    void outputindex() { CCLOG("Index: %d\n", selectedToolIndex); }
    int getToolIndex() { return selectedToolIndex; }

private:
    constexpr static std::string_view kFontPath =
        "assets/fonts/DFHannotateW5-A.ttf";

    cocos2d::Size visible_size_;
    cocos2d::Vec2 visible_origin_;

    cocos2d::Vec2 draggingToolOffset;

    cocos2d::Sprite* background_;                   // background image
    std::vector<cocos2d::LayerColor*> toolBorders;  // Borders for tools
    std::vector<cocos2d::Label*> toolCounts;        // Labels for tool counts
    cocos2d::Sprite* toolIcons[10] = {nullptr};

    int draggingToolIndex = -1;

    void addToolLable();
    void updateToolDisplay();
    void dropCurrentTool();

    // std::unordered_map<int, ItemSprite*> itemSprites;  // Container for item
    // sprites
};

#endif  // TOOL_BAR_H_
