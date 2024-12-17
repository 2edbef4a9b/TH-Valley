#include "frontend/title_screen.h"

#include <cstddef>
#include <cstdint>
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

    AddBackground(kBackgroundImagePath);
    AddTitle();
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

void th_valley::TitleScreen::AddButton(const std::string_view& text,
                                       const float pox_x, const float pos_y,
                                       const std::function<void()>& callback,
                                       const std::string_view& color) {
    // Constants for button appearance.
    constexpr uint8_t kButtonNormalOpacity = 160;
    constexpr uint8_t kButtonSelectedOpacity = 200;
    constexpr uint8_t kButtonPressedOpacity = 230;
    constexpr std::string_view kImageExtension = ".png";
    constexpr std::string_view kImagePattern = "assets/gui/bar/";
    constexpr size_t kFontSize = 36;
    constexpr float kButtonScale = 0.05F;

    // Add the button to the scene.
    auto* button = cocos2d::ui::Button::create(std::string(kImagePattern) +
                                               std::string(color) +
                                               std::string(kImageExtension));

    // Set the zoom scale to 0 so the button doesn't zoom in when clicked.
    button->setZoomScale(kButtonScale);

    // Create the button text.
    auto* label = cocos2d::Label::createWithTTF(
        std::string(text), std::string(kFontPath), kFontSize);
    button->setPosition(cocos2d::Vec2(pox_x, pos_y));
    button->setOpacity(kButtonNormalOpacity);
    label->setPosition(button->getPosition());

    // Lambda to update the button's appearance.
    auto update_button_appearance = [=](const uint8_t opacity,
                                        const float text_scale) {
        button->setOpacity(opacity);
        label->setScale(text_scale);
    };

    // Change button texture on mouse hover.
    auto* mouse_listener = cocos2d::EventListenerMouse::create();
    mouse_listener->onMouseMove =
        [=](const cocos2d::EventMouse* event) mutable {
            if (button->getBoundingBox().containsPoint(
                    event->getLocationInView())) {
                update_button_appearance(kButtonSelectedOpacity,
                                         1.0F + kButtonScale);
            } else {
                update_button_appearance(kButtonNormalOpacity, 1.0F);
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
                    update_button_appearance(kButtonPressedOpacity,
                                             1.0F + kButtonScale);
                    CCLOG("Button touch began");
                    break;
                case cocos2d::ui::Widget::TouchEventType::ENDED:
                    update_button_appearance(kButtonNormalOpacity,
                                             1.0F + kButtonScale);
                    callback();
                    CCLOG("Button touch ended");
                    break;
                case cocos2d::ui::Widget::TouchEventType::CANCELED:
                    update_button_appearance(kButtonNormalOpacity,
                                             1.0F + kButtonScale);
                    CCLOG("Button touch canceled");
                    break;
                case cocos2d::ui::Widget::TouchEventType::MOVED:
                    update_button_appearance(kButtonSelectedOpacity,
                                             1.0F + kButtonScale);
                    CCLOG("Button touch moved");
                    break;
                default:
                    break;
            }
        });

    // Add the button to the scene.
    this->addChild(button, 1);
    this->addChild(label, 2);
}

void th_valley::TitleScreen::AddLabels() {
    const float label_pos_x = visible_origin_.x + (visible_size_.width / 4);
    const float label_pos_y = visible_origin_.y + (visible_size_.height * 0.6F);
    constexpr float kLabelSpacing = 120;

    // Add the buttons to the scene.
    AddButton(
        "SinglePlayer", label_pos_x, label_pos_y,
        []() { CCLOG("GameState Change: TitleScreen -> SinglePlayer"); },
        "purple");
    AddButton(
        "MultiPlayer", label_pos_x, label_pos_y - kLabelSpacing,
        []() { CCLOG("GameState Change: TitleScreen -> MultiPlayer"); },
        "yellow");
    AddButton(
        "Settings", label_pos_x, label_pos_y - (kLabelSpacing * 2),
        []() { CCLOG("GameState Change: TitleScreen -> Settings"); }, "green");
    AddButton(
        "Exit", label_pos_x, label_pos_y - (kLabelSpacing * 3),
        []() { CCLOG("GameState Change: TitleScreen -> Exit"); }, "magenta");
}

void th_valley::TitleScreen::AddTitle() {
    constexpr size_t kTitleFontSize = 72;
    constexpr float kTitleHeightScale = 0.75F;
    // Add the title to the scene.
    auto* title = cocos2d::Label::createWithTTF(
        std::string(kTitleText), std::string(kFontPath), kTitleFontSize);
    title->setPosition(cocos2d::Vec2(
        visible_origin_.x + (visible_size_.width / 4),
        visible_origin_.y + (visible_size_.height * kTitleHeightScale)));
    title->setAlignment(cocos2d::TextHAlignment::CENTER);
    this->addChild(title, 1);
}
