#include "frontend/bag_gui.h"

USING_NS_CC;

bool BagGUI::init() {
    if (!Node::init()) {
        return false;
    }

    visible_size_ = Director::getInstance()->getVisibleSize();
    visible_origin_ = Director::getInstance()->getVisibleOrigin();

    // 初始化背景图片
    background_ = Sprite::create("assets/bag/bag_background.png");
    // background_->setAnchorPoint(Vec2(0, 1));
    background_->setPosition(
        Vec2(visible_origin_.x + visible_size_.width / 2,
             visible_origin_.y + visible_size_.height / 2));
    this->addChild(background_, -1);
    background_->setVisible(false);

    bag_ = new Bag();
    const auto& itemSprites = bag_->getItems();
    for (const auto& [index, item] : itemSprites) {
        CCLOG("GUI Item %s added to bag index %d", item->name.c_str(), index);
    }

    createCloseButton();
    initBagGUI();
    toggleBag();

    return true;
}

void BagGUI::initBagGUI() {
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

    const auto& itemSprites = bag_->getItems();
    auto boxSize = background_->getContentSize().width / 10.0;
    // for (const auto& [index, item] : itemSprites) {
    //     CCLOG("2GUI Item %s added to bag index %d", item->name.c_str(),
    //     index);
    // }

    int index = 0;
    for (const auto& [index, item] : itemSprites) {
        int row = index / 10;
        int col = index % 10;
        if (item != nullptr) {
            auto toolIcon = Sprite::create(item->fileName, item->rect);
            toolIcon->setAnchorPoint(
                Vec2(0.5, 0.5));  // Set anchor point to mid
            toolIcon->setPosition(
                background_->getPosition() +
                Vec2(boxSize / 2 + col * boxSize - boxSize * 5,
                     boxSize / 2 - row * boxSize + boxSize * 1 - 3));
            CCLOG("Tool icon %s position: (%f, %f)", item->name.c_str(),
                  toolIcon->getPosition().x, toolIcon->getPosition().y);
            this->addChild(toolIcon, 3);
            toolIcon->setVisible(true);
            toolIcons[index] = toolIcon;
            itemPositions[item] = toolIcon->getPosition();

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
                    Vec2(toolIcon->getContentSize().width / 2,
                         -4 + -toolIcon->getContentSize().height / 2));
                this->addChild(toolCount);
                toolCounts[index] = toolCount;
            }
        }
    }
}

void BagGUI::onEnter() {
    Layer::onEnter();

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_B) {
            toggleBag();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 改回使用 EventListenerMouse

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(BagGUI::onMouseDown, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(BagGUI::onMouseMove, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(BagGUI::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener,
                                                             this);

    this->scheduleUpdate();
}

void BagGUI::toggleBag() {
    if (isBagOpen) {
        displayBagItems();
    } else {
        hideBagItems();
    }
    isBagOpen = !isBagOpen;
}

void BagGUI::displayBagItems() {
    background_->setVisible(true);
    closeButtonMenu_->setVisible(true);
    initBagGUI();
}

void BagGUI::hideBagItems() {
    for (const auto& icon : toolIcons) {
        if (icon != nullptr) {
            this->removeChild(icon);
            icon == nullptr;
        }
    }
    for (const auto& count : toolCounts) {
        if (count != nullptr) {
            this->removeChild(count);
            count == nullptr;
        }
    }
    closeButtonMenu_->setVisible(false);
    background_->setVisible(false);
}

void BagGUI::createCloseButton() {
    closeButton_ = MenuItemImage::create("assets/bag/close_button_normal.png",
                                         "assets/bag/close_button_selected.png",
                                         [&](Ref* sender) { toggleBag(); });
    closeButton_->setAnchorPoint(Vec2(1, 1));
    closeButton_->setScale(0.3);
    closeButton_->setPosition(
        Vec2(background_->getPositionX() +
                 background_->getContentSize().width / 2 - 1,
             background_->getPositionY() +
                 background_->getContentSize().height / 2 - 1));

    closeButtonMenu_ = Menu::create(closeButton_, nullptr);
    closeButtonMenu_->setPosition(Vec2::ZERO);
    this->addChild(closeButtonMenu_, 2);
    closeButtonMenu_->setVisible(false);
}

void BagGUI::onMouseDown(Event* event) {
    if (isBagOpen) return;  // 背包未打开则不处理
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
    Vec2 location = mouseEvent->getLocationInView();
    location = this->convertToNodeSpace(location);
    CCLOG("Mouse down at (%f, %f)", location.x, location.y);
    const auto& itemSprites = bag_->getItems();
    for (auto& [idx, item] : itemSprites) {
        if (item) {
            Rect boundingBox = toolIcons[idx]->getBoundingBox();
            CCLOG("Item %s bounding box: origin(%f, %f), size(%f, %f)",
                  item->name.c_str(), boundingBox.origin.x,
                  boundingBox.origin.y, boundingBox.size.width,
                  boundingBox.size.height);
            if (boundingBox.containsPoint(location)) {
                selectedItem = toolIcons[idx];
                originalPosition = itemPositions[item];
                draggingItemIndex = idx;
                CCLOG("Item selected at index %d", idx);
                break;
            }
        }
    }
    // toolBar->loadTools();
}

void BagGUI::onMouseMove(Event* event) {
    if (isBagOpen || !selectedItem) return;
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
    Vec2 location = mouseEvent->getLocationInView();
    location = this->convertToNodeSpace(location);
    selectedItem->setPosition(location);
    // toolBar->loadTools();
}

void BagGUI::onMouseUp(Event* event) {
    if (isBagOpen || !selectedItem) return;
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
    Vec2 location = mouseEvent->getLocationInView();
    location = this->convertToNodeSpace(location);

    // 检查是否超出背景范围
    Rect backgroundRect = background_->getBoundingBox();
    if (!backgroundRect.containsPoint(location)) {
        // 超出背景范围，返回原始位置
        selectedItem->setPosition(originalPosition);
        selectedItem = nullptr;
        draggingItemIndex = -1;
        return;
    }

    // 计算实时位置的索引
    int col = (location.x - background_->getPositionX() +
               background_->getContentSize().width / 2) /
              (background_->getContentSize().width / 10);
    int row = (background_->getPositionY() +
               background_->getContentSize().height / 2 - location.y) /
              (background_->getContentSize().width / 10);
    int targetIndex = row * 10 + col;

    CCLOG("Mouse up at (%f, %f), target index: %d", location.x, location.y,
          targetIndex);
    const auto& itemSprites = bag_->getItems();
    if (targetIndex != draggingItemIndex) {
        if (targetIndex == 39) {
            // 扔到垃圾桶
            bag_->removeItem(draggingItemIndex);
        } else {
            // 调换位置
            bag_->swapItems(draggingItemIndex, targetIndex);
        }
    }

    CCLOG("Current itemSprites content:");
    // for (const auto& [key, value] : itemSprites) {
    //     CCLOG("Index: %d, Item: %s", key, value->name.c_str());
    // }

    selectedItem = nullptr;
    draggingItemIndex = -1;
    toolBar->loadTools();
    initBagGUI();
}
