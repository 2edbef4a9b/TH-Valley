#include "game/avatar.h"

#include <utility/logger.h>

#include "2d/CCSprite.h"
#include "CCDirector.h"
#include "game/entity.h"

namespace th_valley {

bool Avatar::init() {
    if (!cocos2d::Sprite::init()) {
        return false;
    }
    if (!Entity::init()) {
        return false;
    }
    InitTexture("koishi");
    InitAnimation("koishi");
    ChangeDirection(Direction::kDown);
    return true;
}

void Avatar::update(float delta) { Entity::update(delta); }

void Avatar::UseTool(std::string_view tool) {
    Logger::GetInstance().LogInfo("Use tool: {}", tool);
    constexpr double kScale = 1.0;
    Direction direction = GetDirection();
    cocos2d::Vec2 tarpos =
        this->getParent()->convertToWorldSpace(this->getPosition());
    tarpos.x *= 0.122;
    tarpos.y *= 0.122;

    if (tool == "Fishingrod") {
        UseFishingRod(tarpos, kScale);
    } else if (tool == "WateringCan") {
        UseWateringCan(tarpos, direction, kScale);
    } else {
        UseOtherTools(tool, tarpos, direction, kScale);
    }
}

void Avatar::UseFishingRod(cocos2d::Vec2 tarpos, double kScale) {
    cocos2d::Sprite* Scene[6];
    tarpos.x += 4;
    tarpos.y -= 4;
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
        Scene[i]->setPosition(cocos2d::Vec2(tarpos.x - 2, tarpos.y));
        Scene[i]->setOpacity(0);
        Scene[i]->setScale(kScale);
        this->addChild(Scene[i]);

        auto fadeIn = cocos2d::FadeIn::create(0.1f);
        auto delay = cocos2d::DelayTime::create(0.8f);
        auto fadeOut = cocos2d::FadeOut::create(0.1f);

        auto lambda = [scenePtr = Scene[i]]() {
            if (scenePtr) {
                scenePtr->removeFromParent();
            }
        };
        auto sequence = cocos2d::Sequence::create(
            cocos2d::DelayTime::create(i * 0.8f), fadeIn, delay, fadeOut,
            cocos2d::CallFunc::create(lambda), nullptr);

        Scene[i]->runAction(sequence);
    }
    Scene[5]->setPosition(cocos2d::Vec2(tarpos.x - 5, tarpos.y + 3.5));
    Scene[5]->setOpacity(0);
    Scene[5]->setScale(kScale);
    this->addChild(Scene[5]);

    auto fadeIn = cocos2d::FadeIn::create(0.1f);
    auto delay = cocos2d::DelayTime::create(2.0f);
    auto fadeOut = cocos2d::FadeOut::create(0.1f);

    auto lambda = [scenePtr = Scene[5]]() {
        if (scenePtr) {
            scenePtr->removeFromParent();
        }
    };
    auto sequence = cocos2d::Sequence::create(
        cocos2d::DelayTime::create(5 * 0.8f), fadeIn, delay, fadeOut,
        cocos2d::CallFunc::create(lambda), nullptr);

    Scene[5]->runAction(sequence);
}

void Avatar::UseWateringCan(cocos2d::Vec2 tarpos, Direction direction,
                            double kScale) {
    int pro = 2;
    if (direction == Direction::kUp) {
        tarpos.y += 8;
        pro = 1;
    } else if (direction == Direction::kDown) {
        tarpos.y -= 8;
    } else if (direction == Direction::kRight) {
        tarpos.x += 8;
        tarpos.y -= 4;
    } else if (direction == Direction::kLeft) {
        tarpos.x -= 8;
        tarpos.y -= 4;
        pro = 1;
    }

    cocos2d::Sprite* tempavatar = cocos2d::Sprite::create(
        "assets/tool/tools.png", cocos2d::Rect(241, 217, 15, 23));
    if (direction == Direction::kUp) {
        tempavatar = cocos2d::Sprite::create(
            "assets/tool/tools.png", cocos2d::Rect(239 - 16, 239 - 18, 16, 18));
    } else if (direction == Direction::kLeft) {
        tempavatar = cocos2d::Sprite::create(
            "assets/tool/tools.png", cocos2d::Rect(289 - 20, 240 - 19, 20, 19));
        tempavatar->setFlippedX(true);
    } else if (direction == Direction::kRight) {
        tempavatar = cocos2d::Sprite::create(
            "assets/tool/tools.png", cocos2d::Rect(289 - 20, 240 - 19, 20, 19));
    }

    tempavatar->setPosition(cocos2d::Vec2(tarpos.x, tarpos.y));
    tempavatar->setOpacity(0);
    tempavatar->setScale(kScale);
    this->addChild(tempavatar, pro);

    auto fadeIn = cocos2d::FadeIn::create(0.05f);
    auto delay = cocos2d::DelayTime::create(1.0f);
    auto fadeOut = cocos2d::FadeOut::create(0.05f);

    auto lambda = [tempavatar]() { tempavatar->removeFromParent(); };
    auto sequence = cocos2d::Sequence::create(
        fadeIn, delay, fadeOut, cocos2d::CallFunc::create(lambda), nullptr);
    tempavatar->runAction(sequence);
}

void Avatar::UseOtherTools(std::string_view tool, cocos2d::Vec2 tarpos,
                           Direction direction, double kScale) {
    int y = 0;
    int pro = 2;
    if (tool == "Hoe") {
        y = 156 - 125;
    } else if (tool == "Axe") {
        y = 156;
    } else if (tool == "Draft") {
        y = 156 - 67;
    } else {
        CCLOG("error : wrong action");
        return;
    }

    if (direction == Direction::kUp) {
        tarpos.y += 8;
        pro = 1;
    } else if (direction == Direction::kDown) {
        tarpos.y -= 8;
    } else if (direction == Direction::kRight) {
        tarpos.x += 4;
    } else if (direction == Direction::kLeft) {
        tarpos.x -= 4;
        pro = 1;
    }

    cocos2d::Sprite* tempavatar = cocos2d::Sprite::create(
        "assets/tool/tools.png", cocos2d::Rect(242, y, 11, 22));
    if (direction == Direction::kUp) {
        tempavatar = cocos2d::Sprite::create("assets/tool/tools.png",
                                             cocos2d::Rect(291, y, 12, 25));
    } else if (direction == Direction::kLeft) {
        tempavatar = cocos2d::Sprite::create("assets/tool/tools.png",
                                             cocos2d::Rect(302, y, 19, 19));
        tempavatar->setFlippedX(true);
    } else if (direction == Direction::kRight) {
        tempavatar = cocos2d::Sprite::create("assets/tool/tools.png",
                                             cocos2d::Rect(302, y, 19, 19));
    }

    tempavatar->setPosition(cocos2d::Vec2(tarpos.x, tarpos.y));
    tempavatar->setOpacity(0);
    tempavatar->setScale(kScale);
    this->addChild(tempavatar, pro);

    auto fadeIn = cocos2d::FadeIn::create(0.05f);
    auto delay = cocos2d::DelayTime::create(0.08f);
    auto fadeOut = cocos2d::FadeOut::create(0.05f);

    auto lambda = [tempavatar]() { tempavatar->removeFromParent(); };
    auto sequence = cocos2d::Sequence::create(
        fadeIn, delay, fadeOut, cocos2d::CallFunc::create(lambda), nullptr);
    tempavatar->runAction(sequence);
}

// void avatarScene::Attacking(std::string weaponTypes) {
//     cocos2d::Vec2 tarpos = Haley.pic->getPosition();
//     cocos2d::Sprite* Scene[2];
//     cocos2d::Sprite* tempavatar;
//     if (weaponTypes == "wand") {
//         for (int i = 0; i < 2; ++i) {
//             Scene[i] = cocos2d::Sprite::create(
//                 "weapon/ranged.png", cocos2d::Rect(198 + 74 * i, 832, 51,
//                 50));
//         }

//         for (int i = 0; i < 2; ++i) {
//             if (direction == Direction::kLeft) {
//                 Scene[i]->setPosition(cocos2d::Vec2(tarpos.x - 50,
//                                                     tarpos.y));  // 设置位置
//             } else if (direction == Direction::kRight) {
//                 Scene[i]->setPosition(cocos2d::Vec2(tarpos.x + 50,
//                                                     tarpos.y));  // 设置位置
//             } else if (direction == Direction::kUp) {
//                 Scene[i]->setPosition(
//                     cocos2d::Vec2(tarpos.x,
//                                   tarpos.y + 50));  // 设置位置
//             } else if (direction == Direction::kDown) {
//                 Scene[i]->setPosition(
//                     cocos2d::Vec2(tarpos.x,
//                                   tarpos.y - 50));  // 设置位置
//             }
//             Scene[i]->setOpacity(0);  // 初始透明度为0
//             this->addChild(Scene[i]);
//             Scene[i]->setScale(0.4f);
//             auto fadeIn = cocos2d::FadeIn::create(0.03f);    // 0.5秒淡入
//             auto delay = cocos2d::DelayTime::create(0.2f);   // 停留1秒
//             auto fadeOut = cocos2d::FadeOut::create(0.03f);  // 0.5秒淡出

//             // 增加基于索引的延迟
//             auto sequence = cocos2d::Sequence::create(
//                 cocos2d::DelayTime::create(i * 0.8f), fadeIn, delay, fadeOut,
//                 cocos2d::CallFunc::create([scenePtr = Scene[i]]() {
//                     if (scenePtr) {
//                         scenePtr->removeFromParent();  // 动作完成后移除
//                     }
//                 }),
//                 nullptr);

//             Scene[i]->runAction(sequence);  // 运行动作序列
//         }
//     } else if (weaponTypes == "sword") {
//         tempavatar = cocos2d::Sprite::create(
//             "weapon/melee.png", cocos2d::Rect(31 - 17, 143 - 16, 17, 16));
//         if (direction == Direction::kLeft) {
//             tempavatar->setFlippedX(true);
//         }
//         if (direction == Direction::kLeft || direction == Direction::kRight)
//         {
//             tarpos.y -= 1;
//             if (direction == Direction::kLeft) {
//                 tarpos.x -= 4;
//             } else if (direction == Direction::kRight) {
//                 tarpos.x += 4;
//             }
//             tempavatar->setPosition(cocos2d::Vec2(tarpos.x,
//                                                   tarpos.y));  // 设置位置
//             tempavatar->setOpacity(0);  // 初始透明度为0
//             this->addChild(tempavatar);
//             auto fadeIn = cocos2d::FadeIn::create(0.05f);    // 0.5秒淡入
//             auto delay = cocos2d::DelayTime::create(0.08f);  // 停留1秒
//             auto fadeOut = cocos2d::FadeOut::create(0.05f);  // 0.5秒淡出

//             // 动作序列
//             auto sequence = cocos2d::Sequence::create(
//                 fadeIn, delay, fadeOut,
//                 cocos2d::CallFunc::create([tempavatar]() {
//                     tempavatar->removeFromParent();  // 动作完成后移除
//                 }),
//                 nullptr);
//             tempavatar->runAction(sequence);  // 运行动作序列
//         }
//     }
// }

void Avatar::RenderMove() {
    Logger::GetInstance().LogInfo(
        "Current "
        "direction: {}",
        static_cast<int>(GetDirection()));
    auto* animation = move_animations_.at(static_cast<int>(GetDirection()));
    auto* repeat = cocos2d::RepeatForever::create(animation);
    if (repeat == nullptr) {
        Logger::GetInstance().LogError(
            "RepeatForever "
            "is nullptr");
        return;
    }
    this->runAction(repeat);
}

void Avatar::ChangeDirection(Direction direction) {
    const Direction current_direction = GetDirection();
    if (current_direction == direction) {
        return;
    }

    SetDirection(direction);
    auto* new_texture = avatar_texture_.down;
    switch (direction) {
        case Direction::kUp:
            new_texture = avatar_texture_.up;
            break;
        case Direction::kDown:
            new_texture = avatar_texture_.down;
            break;
        case Direction::kLeft:
            new_texture = avatar_texture_.left;
            break;
        case Direction::kRight:
            new_texture = avatar_texture_.right;
            break;
        default:
            break;
    }

    cocos2d::Rect frame_rect(0, 0, 128, 128);
    auto* sprite_frame =
        cocos2d::SpriteFrame::createWithTexture(new_texture, frame_rect);
    this->setSpriteFrame(sprite_frame);
}

void Avatar::InitAnimation(std::string_view avatar_name) {
    for (int direction = 0; direction < 4; direction++) {
        cocos2d::Vector<cocos2d::SpriteFrame*> anime_frames;
        for (int index = 0; index < 4; index++) {
            cocos2d::Rect frame_rect(index * 16, direction * 32, 16, 32);
            auto* frame = cocos2d::SpriteFrame::create(
                "assets/tilesheets/Haley.png", frame_rect);
            if (frame) {
                anime_frames.pushBack(
                    frame);  // Add frame to the vector if it is valid
            }
        }

        auto* animation = cocos2d::Animation::createWithSpriteFrames(
            anime_frames, 0.1F);  // Create an animation with the frames.
        auto* animate = cocos2d::Animate::create(animation);
        animate->retain();
        move_animations_.push_back(animate);
    }
}

void Avatar::InitTexture(std::string_view avatar_name) {
    auto* texture_left =
        cocos2d::Director::getInstance()->getTextureCache()->addImage(
            "assets/avatar/" + std::string(avatar_name) + "_left.png");
    auto* texture_right =
        cocos2d::Director::getInstance()->getTextureCache()->addImage(
            "assets/avatar/" + std::string(avatar_name) + "_right.png");
    auto* texture_up =
        cocos2d::Director::getInstance()->getTextureCache()->addImage(
            "assets/avatar/" + std::string(avatar_name) + "_up.png");
    auto* texture_down =
        cocos2d::Director::getInstance()->getTextureCache()->addImage(
            "assets/avatar/" + std::string(avatar_name) + "_down.png");
    avatar_texture_ = {texture_left, texture_right, texture_up, texture_down};
}

}  // namespace th_valley
