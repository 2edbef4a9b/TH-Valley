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

    // 绑定背包数据
    void SetBag(Bag* bag) { bag_ = bag; }

    // 核心功能
    void selectTool(int index);
    void updateToolBar(); // 读取 Bag 的前 10 个格子
    
    std::string getToolName();
    int getToolIndex() { return selectedToolIndex; }

private:
    Bag* bag_ = nullptr;
    int selectedToolIndex = 0;

    static constexpr std::string_view kFontPath = "assets/fonts/DFHannotateW5-A.ttf";
    
    cocos2d::Size visible_size_;
    cocos2d::Vec2 visible_origin_;

    // UI 组件
    cocos2d::Sprite* background_;
    std::vector<cocos2d::LayerColor*> toolBorders;
    std::vector<cocos2d::Label*> toolCounts;
    cocos2d::Sprite* toolIcons[10] = {nullptr};
    cocos2d::Label* MoneyLabel;
    
    // 拖拽相关
    cocos2d::Vec2 draggingToolOffset;
    int draggingToolIndex = -1;

    void addToolLable(); // Label 拼写修正
    void updateToolDisplay(); // 刷新图标显示
    void dropCurrentTool(); // 丢弃逻辑
};

#endif // TOOL_BAR_H_