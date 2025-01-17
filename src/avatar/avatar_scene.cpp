#include <cstddef>
#include <string>

#include "avatar/avatar.h"
#include "sample/sample_scene.h"

namespace th_valley {

void SampleScene::animateRunning(int dir, float flu) {
    // Update animation frames
    cocos2d::Vector<cocos2d::SpriteFrame *> animFrames;
    for (int i = 0; i < 4; i++) {
        // Define the rectangle for each frame
        cocos2d::Rect frameRect(i * 16, dir * 32 + 1, 16, 32);
        auto frame =
            cocos2d::SpriteFrame::create("assets/avatar/Haley.png", frameRect);
        if (frame) {
            animFrames.pushBack(
                frame);  // Add frame to the vector if it is valid
        }
    }
    // Create animation
    auto animation =
        cocos2d::Animation::createWithSpriteFrames(animFrames, flu);
    auto animate = cocos2d::Animate::create(animation);
    Haley->runAction(cocos2d::RepeatForever::create(
        animate));  // Run the animation indefinitely
}

void SampleScene::handleKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                                    cocos2d::Event *event) {
    // B *bPtr = dynamic_cast<B *>(p);  // 安全地将 C* 转换为 B*
    Avatar *Haley_ = dynamic_cast<Avatar *>(Haley);
    isattack = false;

    // Running Deal
    for (int i = 0; i < Alldir.size(); i++) CCLOG("Direction: %d ", Alldir[i]);
    CCLOG("\n\n");

    std::vector<direction>::iterator it;
    switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            isLeft = false;
            for (it = Alldir.begin(); it != Alldir.end(); it++) {
                if (*it == Left) break;
            }
            Alldir.erase(it);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            isRight = false;
            for (it = Alldir.begin(); it != Alldir.end(); it++) {
                if (*it == Right) break;
            }
            Alldir.erase(it);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            isUp = false;
            for (it = Alldir.begin(); it != Alldir.end(); it++) {
                if (*it == Up) break;
            }
            Alldir.erase(it);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            isRight = false;
            for (it = Alldir.begin(); it != Alldir.end(); it++) {
                if (*it == Down) break;
            }
            Alldir.erase(it);
            break;
    }

    Haley->stopAllActions();
    if (Alldir.size() == 0) {
        moved = false;
        Haley->setTextureRect(
            cocos2d::Rect(0, dir * 32 + 1, 16, 32));  // Set to idle frame
    } else {
        dir = Alldir[0];
        CCLOG("Direction: %d\n\n\n", dir);
        animateRunning(dir, 0.06);
    }
}

void SampleScene::handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                                   cocos2d::Event *event) {
    Avatar *Haley_ = dynamic_cast<Avatar *>(Haley);
    cocos2d::Vec2 tarpos = Haley->getPosition();
    switch (keyCode) {
        // move
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            isLeft = true;
            if (!moved) dir = Left;  // Set direction to left
            Alldir.push_back(Left);
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            isRight = true;
            if (!moved) dir = Right;  // Set direction to right
            Alldir.push_back(Right);
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            isUp = true;
            if (!moved) dir = Up;  // Set direction to up
            Alldir.push_back(Up);
            moved = true;  // Mark as moved
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            isDown = true;
            if (!moved) dir = Down;  // Set direction to down
            Alldir.push_back(Down);
            moved = true;  // Mark as moved
            break;
        // cutting
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_E:
        case cocos2d::EventKeyboard::KeyCode::KEY_E:
            if (moved) break;
            tarpos = Haley->getPosition();
            /*if (dir == Up) {
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
            }*/
            CCLOG("=======");
            CCLOG("cutting");
            CCLOG("=======");
            cutting(tarpos);
            moved = false;
            break;
        // mining
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_Z:
        case cocos2d::EventKeyboard::KeyCode::KEY_Z:
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
            mining(tarpos);
            moved = false;
            break;
        // fishing
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_F:
        case cocos2d::EventKeyboard::KeyCode::KEY_F:
            if (dir == Right) {
                tarpos.x += 4;
                CCLOG("=======");
                CCLOG("fishing");
                CCLOG("=======");
                fishing(tarpos);
            }
            moved = false;
            break;
        // cultivating
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_R:
        case cocos2d::EventKeyboard::KeyCode::KEY_R:
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
            cultivating(tarpos);
            moved = false;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_T:
        case cocos2d::EventKeyboard::KeyCode::KEY_T:
            upgradeshow();
            moved = false;
            break;
        default:
            moved = false;
            break;
    }
    if (moved) {
        // CCLOG("Direction: %d\n", dir);
        dir = Alldir[0];
        animateRunning(dir, 0.06);
    }
}
void SampleScene::update(float dt) {
    const float speed = 40.0f;
    float movedistant = speed * dt;
    cocos2d::Vec2 position =
        Haley->getPosition();  // Get the current position of the
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
        /*if (1) {
            Haley->setPosition(position);
        }*/
    }
}
// 砍树函数
void SampleScene::cutting(cocos2d::Vec2 tarpos) {
    // 动作序列：淡入，停留，淡出

    cocos2d::Sprite *tempavatar = cocos2d::Sprite::create(
        "assets/tool/tools.png", cocos2d::Rect(242, 156, 11, 22));
    if (dir == Up) {
        tempavatar = cocos2d::Sprite::create("assets/tool/tools.png",
                                             cocos2d::Rect(291, 156, 12, 25));
    } else if (dir == Left) {
        tempavatar = cocos2d::Sprite::create("assets/tool/tools.png",
                                             cocos2d::Rect(302, 156, 19, 19));
        tempavatar->setScaleX(-1);
    } else if (dir == Right) {
        tempavatar = cocos2d::Sprite::create("assets/tool/tools.png",
                                             cocos2d::Rect(302, 156, 19, 19));
    }
    tempavatar->setPosition(cocos2d::Vec2(tarpos.x, tarpos.y));  // 设置位置
    tempavatar->setOpacity(0);  // 初始透明度为0
    if (dir == Up)
        Haley->addChild(tempavatar, -1);
    else
        Haley->addChild(tempavatar);
    tempavatar->setPosition(cocos2d::Vec2(8, 20));
    auto fadeIn = cocos2d::FadeIn::create(0.05f);    // 0.5秒淡入
    auto delay = cocos2d::DelayTime::create(0.08f);  // 停留1秒
    auto fadeOut = cocos2d::FadeOut::create(0.05f);  // 0.5秒淡出

    // 动作序列
    auto sequence = cocos2d::Sequence::create(
        fadeIn, delay, fadeOut, cocos2d::CallFunc::create([tempavatar]() {
            tempavatar->removeFromParent();  // 动作完成后移除
        }),
        nullptr);
    tempavatar->runAction(sequence);  // 运行动作序列
}

// 耕地函数
void SampleScene::cultivating(cocos2d::Vec2 tarpos) {
    // 动作序列：淡入，停留，淡出
    cocos2d::Sprite *tempavatar = cocos2d::Sprite::create(
        "assets/tool/tools.png", cocos2d::Rect(242, 156 - 125, 11, 22));
    if (dir == Up) {
        tempavatar = cocos2d::Sprite::create(
            "assets/tool/tools.png", cocos2d::Rect(291, 156 - 125, 12, 25));
    } else if (dir == Left) {
        tempavatar = cocos2d::Sprite::create(
            "assets/tool/tools.png", cocos2d::Rect(302, 156 - 125, 19, 19));
        tempavatar->setScaleX(-1);
    } else if (dir == Right) {
        tempavatar = cocos2d::Sprite::create(
            "assets/tool/tools.png", cocos2d::Rect(302, 156 - 125, 19, 19));
    }
    tempavatar->setPosition(cocos2d::Vec2(tarpos.x, tarpos.y));  // 设置位置
    tempavatar->setOpacity(0);  // 初始透明度为0
    this->addChild(tempavatar);
    auto fadeIn = cocos2d::FadeIn::create(0.05f);    // 0.5秒淡入
    auto delay = cocos2d::DelayTime::create(0.08f);  // 停留1秒
    auto fadeOut = cocos2d::FadeOut::create(0.05f);  // 0.5秒淡出

    // 动作序列
    auto sequence = cocos2d::Sequence::create(
        fadeIn, delay, fadeOut, cocos2d::CallFunc::create([tempavatar]() {
            tempavatar->removeFromParent();  // 动作完成后移除
        }),
        nullptr);
    tempavatar->runAction(sequence);  // 运行动作序列
}

// 挖矿函数
void SampleScene::mining(cocos2d::Vec2 tarpos) {
    // 动作序列：淡入，停留，淡出
    cocos2d::Sprite *tempavatar = cocos2d::Sprite::create(
        "assets/tool/tools.png", cocos2d::Rect(242, 156 - 67, 11, 22));
    if (dir == Up) {
        tempavatar = cocos2d::Sprite::create(
            "assets/tool/tools.png", cocos2d::Rect(291, 156 - 67, 12, 25));
    } else if (dir == Left) {
        tempavatar = cocos2d::Sprite::create(
            "assets/tool/tools.png", cocos2d::Rect(302, 156 - 67, 19, 19));
        tempavatar->setScaleX(-1);
    } else if (dir == Right) {
        tempavatar = cocos2d::Sprite::create(
            "assets/tool/tools.png", cocos2d::Rect(302, 156 - 67, 19, 19));
    }
    tempavatar->setPosition(cocos2d::Vec2(tarpos.x, tarpos.y));  // 设置位置
    tempavatar->setOpacity(0);  // 初始透明度为0
    this->addChild(tempavatar);
    auto fadeIn = cocos2d::FadeIn::create(0.05f);    // 0.5秒淡入
    auto delay = cocos2d::DelayTime::create(0.08f);  // 停留1秒
    auto fadeOut = cocos2d::FadeOut::create(0.05f);  // 0.5秒淡出

    // 动作序列
    auto sequence = cocos2d::Sequence::create(
        fadeIn, delay, fadeOut, cocos2d::CallFunc::create([tempavatar]() {
            tempavatar->removeFromParent();  // 动作完成后移除
        }),
        nullptr);
    tempavatar->runAction(sequence);  // 运行动作序列
}

// 钓鱼函数
void SampleScene::fishing(cocos2d::Vec2 tarpos) {
    // 动作序列：淡入，停留，淡出
    cocos2d::Sprite *Scene[6];
    int count = 0;
    Scene[0] = cocos2d::Sprite::create("assets/tool/tools.png",
                                       cocos2d::Rect(0, 295, 35, 31));
    Scene[1] = cocos2d::Sprite::create("assets/tool/tools.png",
                                       cocos2d::Rect(55, 295, 35, 31));
    Scene[2] = cocos2d::Sprite::create("assets/tool/tools.png",
                                       cocos2d::Rect(102, 295, 35, 31));
    Scene[3] = cocos2d::Sprite::create("assets/tool/tools.png",
                                       cocos2d::Rect(160, 295, 35, 31));
    Scene[4] = cocos2d::Sprite::create("assets/tool/tools.png",
                                       cocos2d::Rect(210, 295, 35, 31));
    Scene[5] = cocos2d::Sprite::create("assets/tool/tools.png",
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
    Scene[5]->setOpacity(0);                           // 初始透明度为0
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

void SampleScene::attacking(cocos2d::Vec2 tarpos) {
    // 动作序列：淡入，停留，淡出
    cocos2d::Sprite *Scene[6];
    int count = 0;
    Scene[0] = cocos2d::Sprite::create("assets/tool/tools.png",
                                       cocos2d::Rect(0, 295, 35, 31));
    Scene[1] = cocos2d::Sprite::create("assets/tool/tools.png",
                                       cocos2d::Rect(55, 295, 35, 31));
    Scene[2] = cocos2d::Sprite::create("assets/tool/tools.png",
                                       cocos2d::Rect(102, 295, 35, 31));
    Scene[3] = cocos2d::Sprite::create("assets/tool/tools.png",
                                       cocos2d::Rect(160, 295, 35, 31));
    Scene[4] = cocos2d::Sprite::create("assets/tool/tools.png",
                                       cocos2d::Rect(210, 295, 35, 31));
    Scene[5] = cocos2d::Sprite::create("assets/tool/tools.png",
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
}

void SampleScene::keyboardreading() {
    // Change the default resource path
    // SetResourcePath("assets");
    // Create a keyboard listener
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed =
        [=](cocos2d::EventKeyboard::KeyCode keyCode,
            cocos2d::Event *event) mutable {
            this->handleKeyPressed(keyCode, event);
        };
    keyboardListener->onKeyReleased =
        [=](cocos2d::EventKeyboard::KeyCode keyCode,
            cocos2d::Event *event) mutable {
            this->handleKeyReleased(keyCode, event);  // Handle key release
        };
    // Add the keyboard listener to the event dispatcher
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener,
                                                             Haley);
    this->scheduleUpdate();
}

void SampleScene::SetResourcePath(const std::string &path) {
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

// should be override
void SampleScene::upgradeshow() {
    // 创建 "UPGRADE" 字样
    cocos2d::Vec2 curpos = Haley->getPosition();

    constexpr size_t kFontSize = 12;
    auto *upgradeLabel = cocos2d::Label::createWithTTF(
        "Upgrade!~", "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
    upgradeLabel->setPosition(
        cocos2d::Vec2(curpos.x, curpos.y + 16));  // 设置位置
    upgradeLabel->setOpacity(0);                  // 初始透明度为0
    this->addChild(upgradeLabel);

    // 动作序列：淡入，停留，淡出
    auto fadeIn = cocos2d::FadeIn::create(0.5f);    // 0.5秒淡入
    auto delay = cocos2d::DelayTime::create(1.0f);  // 停留1秒
    auto fadeOut = cocos2d::FadeOut::create(0.5f);  // 0.5秒淡出

    // 动作序列
    auto sequence = cocos2d::Sequence::create(
        fadeIn, delay, fadeOut, cocos2d::CallFunc::create([upgradeLabel]() {
            upgradeLabel->removeFromParent();  // 动作完成后移除
        }),
        nullptr);

    upgradeLabel->runAction(sequence);  // 运行动作序列
    animateRunning(4, 0.03);
    CCLOG("upgrade!");
}

}  // namespace th_valley
