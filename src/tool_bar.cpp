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

    initBag();

    addToolLable();

    // Load tools
    loadTools();

    // Initialize selected tool index
    selectedToolIndex = 0;
    updateToolDisplay();

    return true;
}

void ToolBar::addToolLable() {
    auto boxSize = background_->getContentSize().width / 10.0;
    for (int i = 0; i < 10; ++i) {
        auto toolLabel = Label::createWithTTF(
            std::to_string(i + 1), std::string(kFontPath).c_str(), 6);
        if (i == 9) toolLabel->setString("0");
        toolLabel->setTextColor(Color4B::BLACK);
        toolLabel->setAnchorPoint(Vec2(0, 1));  // Set anchor point to top-left
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
    auto boxSize = background_->getContentSize().width / 10.0;


    int displayCount = 0;
    for (const auto& [index, item] : itemSprites) {
        if (displayCount >= 10) break;

        auto toolIcon = item->sprite;
        toolIcon->setAnchorPoint(
            Vec2(0, 1));  // Set anchor point to top-left
        toolIcon->setPosition(
            Vec2(4 + visible_size_.width / 2 -
                     background_->getContentSize().width / 2 + index * boxSize,
                 -4 + background_->getContentSize().height));
        this->addChild(toolIcon, 25);

        if (item->quantity != 1) {
            auto toolCount =
                Label::createWithTTF(std::to_string(item->quantity),
                                     std::string(kFontPath).c_str(), 8);
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

    //for (int i = 0; i < 10; ++i) {
    //    auto toolBorder =
    //        LayerColor::create(Color4B(255, 255, 0, 128), boxSize - 6,
    //                           background_->getContentSize().height - 6);
    //    toolBorder->setAnchorPoint(Vec2(0, 1));  // Set anchor point to top-left
    //    toolBorder->setPosition(Vec2(4 + visible_size_.width / 2 -
    //                                   background_->getContentSize().width / 2 +
    //                                   i * boxSize,
    //                               -4 + background_->getContentSize().height));
    //    toolBorder->setVisible(false);
    //    this->addChild(toolBorder, 21);
    //    toolBorders.push_back(toolBorder);
    //}
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

void ToolBar::updateToolBar() {
    // Update tool counts or other dynamic elements if needed
}

void ToolBar::dropCurrentTool() {
    if (selectedToolIndex >= 0 && selectedToolIndex < 10 &&
        itemSprites.find(selectedToolIndex) != itemSprites.end()) {
        // Remove the current tool icon, count, and border
        this->removeChild(itemSprites[selectedToolIndex]->sprite);
        if (toolCounts[selectedToolIndex]) {
            this->removeChild(toolCounts[selectedToolIndex]);
            toolCounts[selectedToolIndex] = nullptr;
        }
        delete itemSprites[selectedToolIndex];
        itemSprites.erase(selectedToolIndex);

        // Update selected tool index
        if (selectedToolIndex >= itemSprites.size()) {
            selectedToolIndex = itemSprites.size() - 1;
        }
        updateToolDisplay();
        CCLOG("Dropped tool index: %d", selectedToolIndex);
    }

    // Ensure the selected tool index is valid
    if (selectedToolIndex >= 0 && selectedToolIndex < itemSprites.size()) {
        selectTool(selectedToolIndex);
    } else if (!itemSprites.empty()) {
        selectTool(0);
    } else {
        selectedToolIndex = -1;
    }
}
