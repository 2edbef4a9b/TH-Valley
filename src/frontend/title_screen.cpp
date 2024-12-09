#include "frontend/title_screen.h"

bool th_valley::TitleScreen::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }
    // Get the visible size of the screen.
    visible_size_ = cocos2d::Director::getInstance()->getVisibleSize();
    visible_origin_ = cocos2d::Director::getInstance()->getVisibleOrigin();

    AddBackground("gui/title_screen/background.jpg");

    return true;
}

void th_valley::TitleScreen::AddBackground(
    const std::string_view& background_image) {
    // Load the background image
    auto* background = cocos2d::Sprite::create(std::string(background_image));
    background->setPosition(
        cocos2d::Vec2(visible_origin_.x + (visible_size_.width / 2),
                      visible_origin_.y + (visible_size_.height / 2)));

    // Scale the background to fit the screen
    const float scale_x =
        visible_size_.width / background->getContentSize().width;
    const float scale_y =
        visible_size_.height / background->getContentSize().height;
    background->setScale(scale_x, scale_y);

    // Add the background to the scene.
    this->addChild(background, 0);
}

void th_valley::TitleScreen::AddLabels() {
    // Add the labels to the scene.
}

void th_valley::TitleScreen::AddTitle() {
    // Add the title to the scene.
}
