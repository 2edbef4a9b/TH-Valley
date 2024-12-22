#include "game/avatar.h"

#include <utility/logger.h>

#include "2d/CCSprite.h"
#include "game/entity.h"

namespace th_valley {

bool Avatar::init() {
    if (!cocos2d::Sprite::init()) {
        return false;
    }
    if (!Entity::init()) {
        return false;
    }
    InitAnimation("koishi");
    InitTexture("koishi");
    SetDirection(Direction::kDown);
    return true;
}

void Avatar::update(float delta) { Entity::update(delta); }

void Avatar::RenderAction(std::string_view action) {
    if (action == "move") {
        RenderMove();
    }
}

void Avatar::RenderMove() {
    auto* animation = move_animations_.at(0);
    auto* repeat = cocos2d::RepeatForever::create(animation);
    if (repeat == nullptr) {
        Logger::GetInstance().LogError("RepeatForever is nullptr");
    }
    this->runAction(repeat);
}

void Avatar::ChangeDirection(Direction direction) {
    SetDirection(direction);
    auto* new_texture = this->getTexture();
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

    if (new_texture) {
        cocos2d::Rect frame_rect(0, 0, 128, 128);
        auto* sprite_frame =
            cocos2d::SpriteFrame::createWithTexture(new_texture, frame_rect);
        this->setSpriteFrame(sprite_frame);
    }
}

void Avatar::InitAnimation(std::string_view avatar_name) {
    cocos2d::Vector<cocos2d::SpriteFrame*> anime_frames;
    for (int direction = 0; direction < 4; direction++) {
        for (int index = 0; index < 4; index++) {
            // Define the rectangle for each frame
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
