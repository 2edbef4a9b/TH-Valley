#ifndef TOOL_BAR_H_
#define TOOL_BAR_H_

#include "cocos2d.h"

#include <unordered_map>
#include <vector>
#include <string>
#include "bag.h"

class ToolBar : public cocos2d::Layer {
public:
    ToolBar() = default;
    ~ToolBar() override = default;
    ToolBar(const ToolBar& other) = delete;
    ToolBar& operator=(const ToolBar& other) = delete;
    ToolBar(ToolBar&& other) = delete;
    ToolBar& operator=(ToolBar&& other) = delete;
    bool init() override;
    void onEnter() override;
    //void update(float delta) override;

    CREATE_FUNC(ToolBar);

    void selectTool(int index);
private:
    constexpr static std::string_view kFontPath =
        "assets/fonts/DFHannotateW5-A.ttf";

    cocos2d::Size visible_size_;
    cocos2d::Vec2 visible_origin_;

    cocos2d::Vec2 draggingToolOffset;

    cocos2d::Sprite* background_;  // background image
    std::vector<cocos2d::LayerColor*> toolBorders;  // Borders for tools
    std::vector<cocos2d::Label*> toolCounts;        // Labels for tool counts

    cocos2d::Sprite* toolIcons[10] = {nullptr};

    int selectedToolIndex;
    int draggingToolIndex = -1;

    void addToolLable();
    void updateToolDisplay();
    void dropCurrentTool();
    void loadTools();

    Bag* bag_;
};



#endif  // TOOL_BAR_H_
