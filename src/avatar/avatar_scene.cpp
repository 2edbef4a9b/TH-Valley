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
        auto frame =cocos2d::SpriteFrame::create("avatar/Haley.png", frameRect);
        if (frame) {
            animFrames.pushBack(frame);  // Add frame to the vector if it is valid
        }
    }
    // Create animation
    auto animation =cocos2d::Animation::createWithSpriteFrames(animFrames, float(0.06));
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
    this->addChild(_map);  

    // Create sprite and add it to the scene
    character = cocos2d::Sprite::create("avatar/Haley.png");
    character->setTextureRect(cocos2d::Rect(0, 0, 16, 32));  // Set initial frame to the first frame
    character->setPosition(cocos2d::Vec2(240, 160));  // Set initial position
    this->addChild(character);
    keyboardreading();  // Start reading keyboard input
    return true;  // Ensure to return true to indicate successful initialization
}

void avatarScene::handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,cocos2d::Event *event) {

    moved = false;  
    isattack = false;
    character->stopAllActions();
    character->setTextureRect(cocos2d::Rect(0, dir * 32, 16, 32));  // Set to idle frame
}

void avatarScene::handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    
    // Update direction and position based on key pressed
    switch (keyCode) {
        // move
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            dir = Left;    // Set direction to left
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            dir = Right;   // Set direction to right  
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            dir = Up;      // Set direction to up
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            dir = Down;    // Set direction to down
            moved = true;  // Mark as moved
            break;
        // attack
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_Q:
        case cocos2d::EventKeyboard::KeyCode::KEY_Q:
            if (1) {
                isattack = true;
            }
            Haley->doattack(isattack,dir);
            moved = false;
            break;
        default:
            moved = false;  // If the pressed key is not a movement key, mark as
            break;
    }
    if (moved) {
        animateRunning();
    }
}
void avatarScene::update(float dt)
{
    float movedistant = speed * dt;
    cocos2d::Vec2 position = character->getPosition();  // Get the current position of the
    
    if (moved && dir==2) {
        position.y += movedistant;
    }
    if (moved && dir == 0) {
        position.y -= movedistant;
    }
    if (moved && dir == 3) {
        position.x -= movedistant;
    }
    if (moved && dir == 1) {
        position.x += movedistant;
    }
    if (moved) {      
        CCLOG("=======");
        CCLOG("%lf,%lf", position.x, position.y);  // Log the new position
        CCLOG("=======");
        
        if (1) {
            (Haley->postion_).first = position.x;
            (Haley->postion_).second = position.y;
            character->setPosition(position);
        }
    }
    if (isattack) {
        CCLOG("=======");
        CCLOG("linkstart");
        CCLOG("=======");
        position.x+=0.0001f;
        position.y+=0.0001f;
        blinkTo(position);
        CCLOG("=======");
        CCLOG("%lf,%lf", position.x, position.y);  // Log the new position
        CCLOG("=======");
        position.x -= 0.0001f;
        position.y -= 0.0001f;
        blinkTo(position);
        CCLOG("=======");
        CCLOG("%lf,%lf", position.x, position.y);  // Log the new position
        CCLOG("=======");
        CCLOG("=======");
        CCLOG("linkover");
        CCLOG("=======");
        isattack = false;
    }

}
// 闪现函数
void avatarScene::blinkTo(cocos2d::Vec2 position) {
    // 可以添加闪现特效，例如淡出淡入
    
    auto fadeOut = cocos2d::FadeOut::create(0.1f);  // 0.1秒淡出
    auto fadeIn = cocos2d::FadeIn::create(0.1f);    // 0.1秒淡入
    // 先淡出，然后移动到目标位置，最后淡入
    this->runAction(cocos2d::Sequence::create(fadeOut, cocos2d::CallFunc::create([this, position]() {
            this->setPosition(position);
        }),
        fadeIn, nullptr));
    /*    auto worldPosition = this->getParent()->convertToNodeSpace(targetScreenPosition);
    this->setPosition(worldPosition);*/
}


void avatarScene::keyboardreading() {
    // Change the default resource path
    SetResourcePath("assets");
    // Create a keyboard listener
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode keyCode,
                             cocos2d::Event *event) mutable {
        this->handleKeyPressed(keyCode, event);  
    };
    keyboardListener->onKeyReleased = [=](cocos2d::EventKeyboard::KeyCode keyCode,
            cocos2d::Event *event) mutable {
            this->handleKeyReleased(keyCode, event);  // Handle key release
    };
    // Add the keyboard listener to the event dispatcher
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
                                                             character);
    this->scheduleUpdate();  
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
