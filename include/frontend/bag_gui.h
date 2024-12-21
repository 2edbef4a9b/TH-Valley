#ifndef BAG_GUI_H_
#define BAG_GUI_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "cocos2d.h"
#include "game/bag.h"
#include "tool_bar.h"

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
    // void update(float delta) override;

    CREATE_FUNC(BagGUI);

    ToolBar* toolBar;
    Bag* bag_;

    void toggleBag();  // ��/�رձ���

private:
    constexpr static std::string_view kFontPath =
        "assets/fonts/DFHannotateW5-A.ttf";

    cocos2d::Size visible_size_;
    cocos2d::Vec2 visible_origin_;

    cocos2d::Sprite* background_;          // ����ͼƬ
    cocos2d::MenuItemImage* closeButton_;  // �رհ�ť
    cocos2d::Menu* closeButtonMenu_;

    bool isBagOpen = false;  // �����Ƿ��
    void displayBagItems();
    void hideBagItems();

    void createCloseButton();
    void onMouseDown(cocos2d::Event* event);
    void onMouseMove(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);

    cocos2d::Vec2 originalPosition;           // ��Ʒ��ԭʼλ��
    int draggingItemIndex = -1;               // ������ק��Ʒ������
    cocos2d::Sprite* selectedItem = nullptr;  // ��ǰѡ�е���Ʒ

    cocos2d::Label* toolCounts[40] = {nullptr};  // Labels for tool counts
    cocos2d::Sprite* toolIcons[40] = {nullptr};

    void initBagGUI();

    std::map<ItemSprite*, cocos2d::Vec2> itemPositions;
};

#endif  // BAG_GUI_H_
