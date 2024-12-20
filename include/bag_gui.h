#ifndef BAG_GUI_H_
#define BAG_GUI_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "cocos2d.h"

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

class BagGUI : public cocos2d::Layer {
public:
    BagGUI() = default;
    ~BagGUI() override = default;
    BagGUI(const BagGUI& other) = delete;
    BagGUI& operator=(const BagGUI& other) = delete;
    BagGUI(BagGUI&& other) = delete;
    BagGUI& operator=(BagGUI&& other) = delete;
    bool init() override;
    void onEnter() override;
    //void update(float delta);

    CREATE_FUNC(BagGUI);

    void toggleBag();  // 打开/关闭背包

private:
    constexpr static std::string_view kFontPath =
        "assets/fonts/DFHannotateW5-A.ttf";

    cocos2d::Size visible_size_;
    cocos2d::Vec2 visible_origin_;

    cocos2d::Sprite* background_;         // 背景图片
    cocos2d::MenuItemImage* closeButton_;  // 关闭按钮
    cocos2d::Menu* closeButtonMenu_;

    std::unordered_map<int, ItemSprite*>
        itemSprites;  // Container for item sprites

    bool isBagOpen = true;  // 背包是否打开
    void displayBagItems();
    void hideBagItems();

    void createCloseButton();
    void onMouseDown(cocos2d::Event* event);
    void onMouseMove(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);

    ItemSprite* selectedItem = nullptr;  // 当前选中的物品
    cocos2d::Vec2 originalPosition;               // 物品的原始位置
    int draggingItemIndex = -1;          // 正在拖拽物品的索引

    std::vector<cocos2d::Label*> toolCounts;  // Labels for tool counts
    std::vector<cocos2d::Sprite*> toolIcons;

    void initBagGUI();

    std::map<ItemSprite*, cocos2d::Vec2> itemPositions;

    void initBag() {
        ItemSprite* Axe =
            new ItemSprite("Axe", 1, "Used for chopping wood",
                           cocos2d::Sprite::create(
                               "assets/TileSheets/tools.png",
                               cocos2d::Rect(0 + 5 * 16, 32 + 4 * 32, 16, 16)));
        itemSprites[0] = Axe;
        ItemSprite* Hoe =
            new ItemSprite("Hoe", 3, "Used for chopping wood",
                           cocos2d::Sprite::create(
                               "assets/TileSheets/tools.png",
                               cocos2d::Rect(0 + 5 * 16, 32 + 0 * 32, 16, 16)));
        itemSprites[1] = Hoe;
        ItemSprite* A =
            new ItemSprite("Axe", 1, "Used for chopping wood",
                           cocos2d::Sprite::create(
                               "assets/TileSheets/tools.png",
                               cocos2d::Rect(0 + 5 * 16, 32 + 4 * 32, 16, 16)));
        itemSprites[9] = A;
        ItemSprite* H =
            new ItemSprite("Hoe", 15, "Used for chopping wood",
                           cocos2d::Sprite::create(
                               "assets/TileSheets/tools.png",
                               cocos2d::Rect(0 + 5 * 16, 32 + 0 * 32, 16, 16)));
        itemSprites[15] = H;
        ItemSprite* h =
            new ItemSprite("Hoe", 66, "Used for chopping wood",
                           cocos2d::Sprite::create(
                               "assets/TileSheets/tools.png",
                               cocos2d::Rect(0 + 5 * 16, 32 + 0 * 32, 16, 16)));
        itemSprites[39] = h;

    }
};

#endif  // BAG_GUI_H_
