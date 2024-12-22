#include "game/avatar.h"

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
    InitTexture("koishi");
    SetDirection(Direction::kDown);
    return true;
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
