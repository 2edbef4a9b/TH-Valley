#include <cstddef>
#include <string>
#include "avatar/avatar_scene.h"
#include"avatar/avatar.h"

void avatarScene::animateRunning() {
    // Update animation frames
    cocos2d::Vector<cocos2d::SpriteFrame*> animFrames;
    for (int i = 0; i < 4; i++) {
        // Define the rectangle for each frame
        cocos2d::Rect frameRect(i * 16, dir * 32, 16, 32);  
        auto frame =cocos2d::SpriteFrame::create("avatar/sandy.png", frameRect);
        if (frame) {
            animFrames.pushBack(frame);  // Add frame to the vector if it is valid
        }
    }
    // Create animation
    auto animation =cocos2d::Animation::createWithSpriteFrames(animFrames, float(0.1));
    auto animate = cocos2d::Animate::create(animation);
    character->runAction(cocos2d::RepeatForever::create(animate));  // Run the animation indefinitely
}

bool avatarScene::init() {
    // Initialize parent class
    if (!cocos2d::Scene::init()) {
        return false;
    }
    // Change the default resource path
    SetResourcePath("assets");

    // Load the tiled map
    const auto _map = cocos2d::TMXTiledMap::create("maps/Farm.tmx");
    if (_map) CCLOG("mapLayer found");
    _map->setPosition(cocos2d::Vec2(0, 0));
    _map->setVisible(true);
    this->addChild(_map);  // Add the map to the scene

    // Create sprite and add it to the scene
    character = cocos2d::Sprite::create("avatar/sandy.png");
    character->setTextureRect(
        cocos2d::Rect(0, 0, 16, 32));  // Set initial frame to the first frame
    character->setPosition(cocos2d::Vec2(240, 160));  // Set initial position
    this->addChild(character);
    keyboardreading();  // Start reading keyboard input
    return true;  // Ensure to return true to indicate successful initialization
}

void avatarScene::handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                                    cocos2d::Event *event) {
    moved = false;  // Mark as not moved when a key is released
}

cocos2d::Vec2 avatarScene::handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event,
    cocos2d::Vec2 position) {
    // Define the callback for key pressed
    double a = position.x;
    double b = position.y;
    // Update direction and position based on key pressed
    switch (keyCode) {
        // move
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            dir = Left;    // Set direction to left
            a -= speed;    // Move left
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            dir = Right;   // Set direction to right
            a += speed;    // Move right
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            dir = Up;      // Set direction to up
            b += speed;    // Move up
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            dir = Down;    // Set direction to down
            b -= speed;    // Move down
            moved = true;  // Mark as moved
            break;
        // attack
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_Q:
        case cocos2d::EventKeyboard::KeyCode::KEY_Q:
            moved = false;




        default:

            moved = false;  // If the pressed key is not a movement key, mark as
                            // not moved
            break;
    }
    animateRunning();            // Trigger the running animation
    return cocos2d::Vec2(a, b);  // Return the updated position
}

void avatarScene::keyboardreading() {
    // Change the default resource path
    SetResourcePath("assets");
    // Create a keyboard listener
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    // Define the callback for key pressed
    cocos2d::Vec2 position =
        character->getPosition();  // Get the current position of the sprite
    cocos2d::Vec2 positionout;
    keyboardListener
        ->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode keyCode,
                             cocos2d::Event *event) mutable {
        positionout.set(this->handleKeyPressed(keyCode, event, position).x,
                        this->handleKeyPressed(keyCode, event, position).y);
        CCLOG("=======");
        CCLOG("%lf,%lf", positionout.x, positionout.y);  // Log the new position
        CCLOG("=======");

        character->setPosition(positionout);  // Update the sprite's position
        position =
            character
                ->getPosition();  // Refresh the current position of the sprite
    };
    keyboardListener->onKeyReleased =
        [=](cocos2d::EventKeyboard::KeyCode keyCode,
            cocos2d::Event *event) mutable {
            this->handleKeyReleased(keyCode, event);  // Handle key release
        };
    CCLOG("=======");
    // Add the keyboard listener to the event dispatcher
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
                                                             character);
    // Schedule a function to update idle state
    schedule(
        [this](float dt) {
            // Update idle time if no input
            if (!moved) {
                idleTime += dt;  // Increment idle time
                if (idleTime >= idleThreshold) {
                    // Stop animation and set to idle frame
                    character->stopAllActions();
                    character->setTextureRect(
                        cocos2d::Rect(0, 0, 16, 32));  // Set to idle frame
                }
            } else {
                idleTime = 0.0f;  // Reset idle time if moved
            }
        },
        "idle_update_key");
}


void avatarScene::SetResourcePath(const std::string &path) {
    auto *file_utils = cocos2d::FileUtils::getInstance();

    // Get the current resource root absolute path.
    std::string resource_root_path =
        cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();

    // Replace the last directory name with the new path.
    const size_t pos =
        resource_root_path.find_last_of('/', resource_root_path.size() - 2);
    if (pos != std::string::npos) {
        resource_root_path.replace(pos + 1, std::string::npos, path + "/");
    } else {
        resource_root_path = path + "/";
    }

    // Update the default resource root path.
    file_utils->setDefaultResourceRootPath(resource_root_path);
}
