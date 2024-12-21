#ifndef TOOL_BAR_H_
#define TOOL_BAR_H_

#include "cocos2d.h"

#include <unordered_map>
#include <vector>
#include <string>

struct ItemSprite {
    std::string name;
    int quantity;
    std::string description;
    cocos2d::Sprite* sprite;
    ItemSprite(std::string name, int quantity, std::string description,
               cocos2d::Sprite* sprite)
        : name(name),
          quantity(quantity),
          description(description),
          sprite(sprite) {}
};


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

    CREATE_FUNC(ToolBar);

    void selectTool(int index);
    void updateToolBar();
    std::string getToolName();
    void outputindex() { CCLOG("Index: %d\n", selectedToolIndex); }

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
    void loadTools();
    void updateToolDisplay();
    void dropCurrentTool();

    std::unordered_map<int, ItemSprite*> itemSprites;  // Container for item sprites
    Bag* bag_;

    void initBag() {
        ItemSprite* Axe =
            new ItemSprite("Axe", 1, "Used for chopping wood",
                           cocos2d::Sprite::create("assets/TileSheets/tools.png",
                               cocos2d::Rect(0 + 5 * 16, 32 + 4 * 32, 16, 16)));
        itemSprites[0] = Axe;
        ItemSprite* Hoe =
            new ItemSprite("Hoe", 3, "Used for chopping wood",
                           cocos2d::Sprite::create("assets/TileSheets/tools.png",
                               cocos2d::Rect(0 + 5 * 16, 32 + 0 * 32, 16, 16)));
        itemSprites[1] = Hoe;
    }
};



#endif  // TOOL_BAR_H_
