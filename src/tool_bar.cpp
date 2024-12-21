#include "tool_bar.h"

USING_NS_CC;

bool ToolBar::init() {
    if (!Node::init()) {
        return false;
    }

    visible_size_ = Director::getInstance()->getVisibleSize();
    visible_origin_ = Director::getInstance()->getVisibleOrigin();

    // Load toolbar background
    background_ = Sprite::create("assets/tool_bar/toolbar_background.png");
    background_->setAnchorPoint(Vec2(0.5, 0));
    background_->setPosition(visible_size_.width / 2, 0);

    this->addChild(background_, 20);

    bag_ = new Bag();

    addToolLable();
    // Load tools
    loadTools();




    return true;
}

void ToolBar::addToolLable() {
    auto boxSize = background_->getContentSize().width / 10.0;
    for (int i = 0; i < 10; ++i) {
        auto toolLabel = Label::createWithTTF(
            std::to_string(i + 1), std::string(kFontPath).c_str(), 30);
        if (i == 9) toolLabel->setString("0");
        toolLabel->setScale(0.2);
        toolLabel->setTextColor(Color4B::GRAY);
        toolLabel->setAnchorPoint(Vec2(0, 1.5));  // Set anchor point to top-left
        toolLabel->setPosition(
            Vec2(4 + visible_size_.width / 2 - background_->getContentSize().width / 2 +
                 i * boxSize, background_->getContentSize().height));
        this->addChild(toolLabel, 30);
    }
}


void ToolBar::onEnter() {
    Node::onEnter();

    // Add keyboard event listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode,
                                            Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_Q) {
            // Drop the current tool
            dropCurrentTool();
        } else if (keyCode >= EventKeyboard::KeyCode::KEY_1 &&
                   keyCode <= EventKeyboard::KeyCode::KEY_9) {
            int index = static_cast<int>(keyCode) -
                        static_cast<int>(EventKeyboard::KeyCode::KEY_1);
            selectTool(index);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_0)
            selectTool(9);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
                                                             this);
}

void ToolBar::loadTools() {
    for (auto icon : toolIcons) {
        if (icon != nullptr) {
            this->removeChild(icon);
            icon == nullptr;
        }
    }
    for (auto count : toolCounts) {
        if (count != nullptr) {
            this->removeChild(count);
            count == nullptr;
        }
    }


    auto boxSize = background_->getContentSize().width / 10.0;
    const auto& itemSprites = bag_->getItems();
    for (const auto& [index, item] : itemSprites) {
        CCLOG("Item %s added to bag index %d", item->name.c_str(), index);
    }

    int displayCount = 0;
    for (const auto& [index, item] : itemSprites) {
        if (displayCount >= 10) break;

        auto toolIcon = Sprite::create(item->fileName, item->rect);
        toolIcon->setAnchorPoint(
            Vec2(0, 1));  // Set anchor point to top-left
        toolIcon->setPosition(
            Vec2(4 + visible_size_.width / 2 -
                     background_->getContentSize().width / 2 + index * boxSize,
                 -4 + background_->getContentSize().height));
        this->addChild(toolIcon, 25);
        toolIcons[index] = toolIcon;

        if (item->quantity != 1) {
            auto toolCount =
                Label::createWithTTF(std::to_string(item->quantity),
                                     std::string(kFontPath).c_str(), 40);
            toolCount->setScale(0.2);
            toolCount->setTextColor(Color4B::BLACK);
            toolCount->setAnchorPoint(
                Vec2(1, 0));  // Set anchor point to bottom-right
            toolCount->setPosition(
                toolIcon->getPosition() +
                Vec2(toolIcon->getContentSize().width,
                     -4 + -toolIcon->getContentSize().height));
            this->addChild(toolCount, 30);
            toolCounts.push_back(toolCount);
        } else {
            toolCounts.push_back(nullptr);
        }

        displayCount++;
    }

    for (int i = 0; i < 10; ++i) {
        auto toolBorder =
            LayerColor::create(Color4B(255, 255, 0, 128), boxSize - 6,
                               boxSize - 6);
        toolBorder->setAnchorPoint(Vec2(0, 0));  // Set anchor point to top-left
        toolBorder->setPosition(
            Vec2(4 + visible_size_.width / 2 -
                     background_->getContentSize().width / 2 + i * boxSize,
                 4));
        toolBorder->setVisible(false);
        this->addChild(toolBorder,
                       21);  // Ensure the border is below the tool icons
        toolBorders.push_back(toolBorder);
    }
}

void ToolBar::selectTool(int index) {
    if (index >= 0 && index < 10) {
        selectedToolIndex = index;
        updateToolDisplay();
        CCLOG("Selected tool index: %d", index);
    }
}

void ToolBar::updateToolDisplay() {
    for (size_t i = 0; i < toolBorders.size(); ++i) {
        toolBorders[i]->setVisible(i == selectedToolIndex);
    }
}


void ToolBar::dropCurrentTool() {
    if (selectedToolIndex >= 0 && selectedToolIndex < 10) {
        if (toolIcons[selectedToolIndex] != nullptr && bag_) {
            // 从 Bag 中移除对应的物品
            bag_->removeItem(selectedToolIndex);
            // 更新工具栏显示
            loadTools();
            CCLOG("Dropped tool index: %d", selectedToolIndex);
        }
    }

    const auto& itemSprites = bag_->getItems();
    for (const auto& [index, item] : itemSprites) {
        CCLOG("Item %s added to bag index %d", item->name.c_str(), index);
    }
    selectedToolIndex = -1;

}
