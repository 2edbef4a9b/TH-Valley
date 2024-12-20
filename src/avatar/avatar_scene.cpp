#include <cstddef>
#include <string>
#include "avatar/avatar_scene.h"
#include"avatar/avatar.h"

void avatarScene::Moving(int dir,float flu) {
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
    auto animation =cocos2d::Animation::createWithSpriteFrames(animFrames, flu);
    auto animate = cocos2d::Animate::create(animation);
    Haley->runAction(cocos2d::RepeatForever::create(
        animate));  // Run the animation indefinitely
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
    Haley = cocos2d::Sprite::create("avatar/Haley.png");
    Haley->setTextureRect(
        cocos2d::Rect(0, 0, 16, 32));  // Set initial frame to the first frame
    Haley->setPosition(cocos2d::Vec2(240, 160));  // Set initial position
    this->addChild(Haley);
    keyboardreading();  // Start reading keyboard input
    return true;  // Ensure to return true to indicate successful initialization
}

void avatarScene::handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,cocos2d::Event *event) {

    Avatar *Haley_ = dynamic_cast<Avatar *>(Haley);
    moved = false;  
    isattack = false;
    Haley->stopAllActions();
    Haley->setTextureRect(cocos2d::Rect(0, dir * 32, 16, 32));  // Set to idle frame
}

void avatarScene::handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    Avatar *Haley_ = dynamic_cast<Avatar *>(Haley);
    cocos2d::Vec2 tarpos = Haley->getPosition();
    switch (keyCode) {
        // move
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            dir = Left;  // Set direction to left
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            dir = Right;  // Set direction to right  
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            dir = Up;  // Set direction to up
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            dir = Down;  // Set direction to down
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_G:
        case cocos2d::EventKeyboard::KeyCode::KEY_G:
            Attacking("sword");
            moved = false;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_H:
        case cocos2d::EventKeyboard::KeyCode::KEY_H:
            Attacking("wand");
            moved = false;  // Mark as moved
            break;
        // cutting
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_E:
        case cocos2d::EventKeyboard::KeyCode::KEY_E: 
            Working("Axe");
            moved = false;
            break;
        // mining
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_Z:
        case cocos2d::EventKeyboard::KeyCode::KEY_Z:
            Working("Draft");
            moved = false;
            break;
        // fishing
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_F:
        case cocos2d::EventKeyboard::KeyCode::KEY_F:
            Working("Fishingrod");
            moved = false;
            break;
        // cultivating
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_R:
        case cocos2d::EventKeyboard::KeyCode::KEY_R:
            Working("Hoe");
            moved = false;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_T:
        case cocos2d::EventKeyboard::KeyCode::KEY_T:
            upgrading();
            moved = false;
            break;
        default:
            moved =false;  
            break;
    }
    if (moved) {
        Moving(dir, 0.06);
    }
}


void avatarScene::Differ(std::string handy) {






}






void avatarScene::update(float dt)
{
    const float speed = 40.0f;
    float movedistant = speed * dt;
    cocos2d::Vec2 position =Haley->getPosition();  // Get the current position of the
    Avatar *Haley_ = dynamic_cast<Avatar *>(Haley);
    if (moved && dir == 2) {
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
            Haley->setPosition(position);
        }
    }


}

void avatarScene::Working(std::string action) {
    Avatar *Haley_ = dynamic_cast<Avatar *>(Haley);
    cocos2d::Vec2 tarpos = Haley->getPosition();
    cocos2d::Sprite *tempavatar;
    cocos2d::Sprite *Scene[6];
    if (action == "Fishingrod") {
        CCLOG("=======");
        CCLOG("fishing");
        CCLOG("=======");
        tarpos.x+=4;
        tarpos.y -= 4;
        Scene[0] = cocos2d::Sprite::create("tool/tools.png",
                                           cocos2d::Rect(0, 295, 35, 31));
        Scene[1] = cocos2d::Sprite::create("tool/tools.png",
                                           cocos2d::Rect(55, 295, 35, 31));
        Scene[2] = cocos2d::Sprite::create("tool/tools.png",
                                           cocos2d::Rect(102, 295, 35, 31));
        Scene[3] = cocos2d::Sprite::create("tool/tools.png",
                                           cocos2d::Rect(160, 295, 35, 31));
        Scene[4] = cocos2d::Sprite::create("tool/tools.png",
                                           cocos2d::Rect(210, 295, 35, 31));
        Scene[5] = cocos2d::Sprite::create("tool/tools.png",
                                           cocos2d::Rect(256, 295, 35, 31));

        for (int i = 0; i < 5; ++i) {
            Scene[i]->setPosition(
                cocos2d::Vec2(tarpos.x - 2, tarpos.y));  // 设置位置
            Scene[i]->setOpacity(0);                     // 初始透明度为0
            this->addChild(Scene[i]);

            auto fadeIn = cocos2d::FadeIn::create(0.1f);    // 0.5秒淡入
            auto delay = cocos2d::DelayTime::create(0.8f);  // 停留1秒
            auto fadeOut = cocos2d::FadeOut::create(0.1f);  // 0.5秒淡出

            // 增加基于索引的延迟
            auto sequence = cocos2d::Sequence::create(
                cocos2d::DelayTime::create(i * 0.8f), fadeIn, delay, fadeOut,
                cocos2d::CallFunc::create([scenePtr = Scene[i]]() {
                    if (scenePtr) {
                        scenePtr->removeFromParent();  // 动作完成后移除
                    }
                }),
                nullptr);

            Scene[i]->runAction(sequence);  // 运行动作序列
        }
        Scene[5]->setPosition(
            cocos2d::Vec2(tarpos.x - 5, tarpos.y + 3.5));  // 设置位置
        Scene[5]->setOpacity(0);  // 初始透明度为0
        this->addChild(Scene[5]);

        auto fadeIn = cocos2d::FadeIn::create(0.1f);    // 0.5秒淡入
        auto delay = cocos2d::DelayTime::create(2.0f);  // 停留1秒
        auto fadeOut = cocos2d::FadeOut::create(0.1f);  // 0.5秒淡出

        // 增加基于索引的延迟
        auto sequence = cocos2d::Sequence::create(
            cocos2d::DelayTime::create(5 * 0.8f), fadeIn, delay, fadeOut,
            cocos2d::CallFunc::create([scenePtr = Scene[5]]() {
                if (scenePtr) {
                    scenePtr->removeFromParent();  // 动作完成后移除
                }
            }),
            nullptr);

        Scene[5]->runAction(sequence);  // 运行动作序列
    }
    else {
        int y = 0;
        if (action == "Hoe") {
            y = 156 - 125;
            if (dir == Up) {
                tarpos.y += 8;
            }
            if (dir == Down) {
                tarpos.y -= 8;
            }
            if (dir == Right) {
                tarpos.x += 4;
            }
            if (dir == Left) {
                tarpos.x -= 4;
            }
            CCLOG("=======");
            CCLOG("cultivating");
            CCLOG("=======");
        } else if (action == "Axe") {
            y = 156;
            if (dir == Up) {
                tarpos.y += 8;
            }
            if (dir == Down) {
                tarpos.y -= 8;
            }
            if (dir == Right) {
                tarpos.x += 4;
            }
            if (dir == Left) {
                tarpos.x -= 4;
            }
            CCLOG("=======");
            CCLOG("cutting");
            CCLOG("=======");
        } else if (action == "Draft") {
            y = 156 - 67;
            if (dir == Up) {
                tarpos.y += 8;
            }
            if (dir == Down) {
                tarpos.y -= 8;
            }
            if (dir == Right) {
                tarpos.x += 4;
            }
            if (dir == Left) {
                tarpos.x -= 4;
            }
            CCLOG("=======");
            CCLOG("mining");
            CCLOG("=======");
        }
        if (y != 0)
        // 动作序列：淡入，停留，淡出
        {
            tempavatar = cocos2d::Sprite::create("tool/tools.png",
                                                 cocos2d::Rect(242, y, 11, 22));
            if (dir == Up) {
                tempavatar = cocos2d::Sprite::create(
                    "tool/tools.png", cocos2d::Rect(291, y, 12, 25));
            } else if (dir == Left) {
                tempavatar = cocos2d::Sprite::create(
                    "tool/tools.png", cocos2d::Rect(302, y, 19, 19));
                tempavatar->setScaleX(-1);
            } else if (dir == Right) {
                tempavatar = cocos2d::Sprite::create(
                    "tool/tools.png", cocos2d::Rect(302, y, 19, 19));
            }
            tempavatar->setPosition(
                cocos2d::Vec2(tarpos.x, tarpos.y));  // 设置位置
            tempavatar->setOpacity(0);               // 初始透明度为0
            this->addChild(tempavatar);
            auto fadeIn = cocos2d::FadeIn::create(0.05f);    // 0.5秒淡入
            auto delay = cocos2d::DelayTime::create(0.08f);  // 停留1秒
            auto fadeOut = cocos2d::FadeOut::create(0.05f);  // 0.5秒淡出

            // 动作序列
            auto sequence = cocos2d::Sequence::create(
                fadeIn, delay, fadeOut,
                cocos2d::CallFunc::create([tempavatar]() {
                    tempavatar->removeFromParent();  // 动作完成后移除
                }),
                nullptr);
            tempavatar->runAction(sequence);  // 运行动作序列
        } else {
            CCLOG("error : wrong action");
        }
    }
}

void avatarScene::Attacking(std::string weaponTypes) {
    Avatar *Haley_ = dynamic_cast<Avatar *>(Haley);
    cocos2d::Vec2 tarpos = Haley->getPosition();
    cocos2d::Sprite *Scene[2];
    cocos2d::Sprite *tempavatar;
    if (weaponTypes == "wand") {
        for (int i = 0; i < 2; ++i) {
            Scene[i] = cocos2d::Sprite::create(
                "weapon/ranged.png", cocos2d::Rect(198 + 74 * i, 832, 51, 50));
        }

        for (int i = 0; i < 2; ++i) {
            if (dir == Left) {
                Scene[i]->setPosition(
                    cocos2d::Vec2(tarpos.x - 50, tarpos.y));  // 设置位置
            } else if (dir == Right) {
                Scene[i]->setPosition(
                    cocos2d::Vec2(tarpos.x + 50, tarpos.y));  // 设置位置
            } else if (dir == Up) {
                Scene[i]->setPosition(
                    cocos2d::Vec2(tarpos.x, tarpos.y + 50));  // 设置位置
            } else if (dir == Down) {
                Scene[i]->setPosition(
                    cocos2d::Vec2(tarpos.x, tarpos.y - 50));  // 设置位置
            }
            Scene[i]->setOpacity(0);  // 初始透明度为0
            this->addChild(Scene[i]);
            Scene[i]->setScale(0.4f);
            auto fadeIn = cocos2d::FadeIn::create(0.03f);    // 0.5秒淡入
            auto delay = cocos2d::DelayTime::create(0.2f);   // 停留1秒
            auto fadeOut = cocos2d::FadeOut::create(0.03f);  // 0.5秒淡出

            // 增加基于索引的延迟
            auto sequence = cocos2d::Sequence::create(
                cocos2d::DelayTime::create(i * 0.8f), fadeIn, delay, fadeOut,
                cocos2d::CallFunc::create([scenePtr = Scene[i]]() {
                    if (scenePtr) {
                        scenePtr->removeFromParent();  // 动作完成后移除
                    }
                }),
                nullptr);

            Scene[i]->runAction(sequence);  // 运行动作序列
        }
    } else if (weaponTypes == "sword") {
        tempavatar = cocos2d::Sprite::create(
            "weapon/melee.png", cocos2d::Rect(31 - 17, 143 - 16, 17, 16));
        if (dir == Left) {
            tempavatar->setScaleX(-1);
        }
        if (dir == Left || dir == Right) {
            tarpos.y -= 1;
            if (dir == Left) {
                tarpos.x -= 4;
            } else if (dir == Right) {
                tarpos.x += 4;
            }
            tempavatar->setPosition(
                cocos2d::Vec2(tarpos.x, tarpos.y));  // 设置位置
            tempavatar->setOpacity(0);               // 初始透明度为0
            this->addChild(tempavatar);
            auto fadeIn = cocos2d::FadeIn::create(0.05f);    // 0.5秒淡入
            auto delay = cocos2d::DelayTime::create(0.08f);  // 停留1秒
            auto fadeOut = cocos2d::FadeOut::create(0.05f);  // 0.5秒淡出

            // 动作序列
            auto sequence = cocos2d::Sequence::create(
                fadeIn, delay, fadeOut,
                cocos2d::CallFunc::create([tempavatar]() {
                    tempavatar->removeFromParent();  // 动作完成后移除
                }),
                nullptr);
            tempavatar->runAction(sequence);  // 运行动作序列
        }

    }






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
                                                             Haley);
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

void avatarScene::upgrading() {
    // 创建 "UPGRADE" 字样
    cocos2d::Vec2 curpos = Haley->getPosition();
   
    constexpr size_t kFontSize = 12;
    auto *upgradeLabel = cocos2d::Label::createWithTTF("Upgrade!~", "fonts/DFHannotateW5-A.ttf", kFontSize);
    upgradeLabel->setPosition(cocos2d::Vec2(curpos.x, curpos.y+16));  // 设置位置
    upgradeLabel->setOpacity(0);                         // 初始透明度为0
    this->addChild(upgradeLabel);

    // 动作序列：淡入，停留，淡出
    auto fadeIn = cocos2d::FadeIn::create(0.5f);    // 0.5秒淡入
    auto delay = cocos2d::DelayTime::create(1.0f);  // 停留1秒
    auto fadeOut = cocos2d::FadeOut::create(0.5f);  // 0.5秒淡出

    // 动作序列
    auto sequence = cocos2d::Sequence::create(fadeIn, delay, fadeOut, cocos2d::CallFunc::create([upgradeLabel]() {
            upgradeLabel->removeFromParent();  // 动作完成后移除
        }),
        nullptr);

    upgradeLabel->runAction(sequence);  // 运行动作序列
    Moving(4,0.03);
    CCLOG("upgrade!");
}
