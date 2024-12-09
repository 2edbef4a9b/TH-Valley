#include "frontend/title_screen.h"

bool th_valley::TitleScreen::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }

    const auto visible_size =
        cocos2d::Director::getInstance()->getVisibleSize();
    const auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // Load the background image
    auto* background =
        cocos2d::Sprite::create("gui/title_screen/background.png");
    background->setPosition(
        cocos2d::Vec2(origin.x + (visible_size.width / 2),
                      origin.y + (visible_size.height / 2)));

    // Scale the background to fit the screen
    const float scale_x =
        visible_size.width / background->getContentSize().width;
    const float scale_y =
        visible_size.height / background->getContentSize().height;
    background->setScale(scale_x, scale_y);

    // Add the background to the scene.
    this->addChild(background, 0);

    auto* title = cocos2d::Label::createWithTTF(
        "TH Valley", "fonts/DFHannotateW5-A.ttf", 48);
    title->setPosition(cocos2d::Vec2(
        origin.x + (visible_size.width / 2),
        origin.y + visible_size.height - title->getContentSize().height));
    this->addChild(title, 1);

    return true;
}
