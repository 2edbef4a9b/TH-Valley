#include "frontend/title_screen.h"

#include <string_view>

#include "CCEventDispatcher.h"
#include "CCPlatformMacros.h"
#include "ui/CocosGUI.h"

bool th_valley::TitleScreen::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }
    // Get the visible size of the screen.
    visible_size_ = cocos2d::Director::getInstance()->getVisibleSize();
    visible_origin_ = cocos2d::Director::getInstance()->getVisibleOrigin();

    AddBackground("gui/title/background_blank.png");
    AddLabels();

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

void th_valley::TitleScreen::AddButton() {
    // Add the button to the scene.
    auto* button = cocos2d::ui::Button::create("gui/bar/purplebar_normal.png");

    // Set the zoom scale to 0 so the button doesn't zoom in when clicked.
    button->setZoomScale(0.0F);

    // Create the button text.
    button->setTitleFontName("fonts/DFHannotateW5-A.ttf");
    button->setTitleText("Start");
    button->setTitleColor(cocos2d::Color3B::WHITE);

    button->setPosition(
        cocos2d::Vec2(visible_origin_.x + (visible_size_.width / 2),
                      visible_origin_.y + (visible_size_.height / 2)));

    // Change button texture on mouse hover.
    auto* mouse_listener = cocos2d::EventListenerMouse::create();
    mouse_listener->onMouseMove = [=](cocos2d::EventMouse* event) mutable {
        auto mouse_pos = event->getLocationInView();
        mouse_pos = cocos2d::Director::getInstance()->convertToGL(mouse_pos);

        if (button->getBoundingBox().containsPoint(mouse_pos)) {
            button->loadTextureNormal("gui/bar/purplebar_selected.png");
        } else {
            button->loadTextureNormal("gui/bar/purplebar_normal.png");
        }
    };

    // Add the mouse listener to the button.
    cocos2d::Director::getInstance()
        ->getEventDispatcher()
        ->addEventListenerWithSceneGraphPriority(mouse_listener, button);

    // Add event listener for different states.
    button->addTouchEventListener(
        [=](cocos2d::Ref* sender,
            const cocos2d::ui::Widget::TouchEventType type) mutable {
            switch (type) {
                case cocos2d::ui::Widget::TouchEventType::BEGAN:
                    button->loadTextureNormal("gui/bar/purplebar_pressed.png");
                    CCLOG("Button touch began");
                    break;
                case cocos2d::ui::Widget::TouchEventType::ENDED:
                    button->loadTextureNormal("gui/bar/purplebar_selected.png");
                    CCLOG("Button touch ended");
                    break;
                case cocos2d::ui::Widget::TouchEventType::CANCELED:
                    button->loadTextureNormal("gui/bar/purplebar_normal.png");
                    CCLOG("Button touch canceled");
                    break;
                case cocos2d::ui::Widget::TouchEventType::MOVED:
                    button->loadTextureNormal("gui/bar/purplebar_pressed.png");
                    CCLOG("Button touch moved");
                    break;
                default:
                    break;
            }
        });

    // Add the button to the scene.
    this->addChild(button, 1);
}

void th_valley::TitleScreen::AddLabels() {
    // Add the labels to the scene.
    AddButton();
}

void th_valley::TitleScreen::AddTitle() {
    // Add the title to the scene.
}
