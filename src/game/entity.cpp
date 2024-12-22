#include "game/entity.h"

#include "utility/logger.h"

namespace th_valley {

void Entity::SetState(EntityState state) { state_ = state; }

Entity::EntityState Entity::GetState() const { return state_; }

void Entity::SetDirection(Direction direction) { direction_ = direction; }

Entity::Direction Entity::GetDirection() const { return direction_; }

bool Entity::init() {
    if (!cocos2d::Sprite::init()) {
        return false;
    }
    auto* texture =
        cocos2d::Director::getInstance()->getTextureCache()->addImage(
            "assets/avatar/koishi_left.png");
    auto* sprite_frame = cocos2d::SpriteFrame::createWithTexture(
        texture, cocos2d::Rect(0, 0, 128, 128));
    this->setSpriteFrame(sprite_frame);
    this->setScale(32.0F / 128.0F);
    this->setAnchorPoint(cocos2d::Vec2(0.5F, 0.0F));

    return true;
}

void Entity::update(float delta) {
    constexpr float kMoveSpeed = 100.0F;
    if (state_ == EntityState::kAction) {
        Logger::GetInstance().LogInfo("Action");
    }
}

void Entity::InitEntity(cocos2d::Node* parent) {
    if (!init()) {
        return;
    }
    parent->addChild(this, 0);
}

void Entity::ChangeDirection(Direction direction) {
    direction_ = direction;
    switch (direction_) {
        case Direction::kUp:
            this->setRotation(0);
            this->setFlippedX(false);
            break;
        case Direction::kDown:
            this->setRotation(0);
            this->setFlippedX(false);
            break;
        case Direction::kLeft:
            this->setRotation(0);
            this->setFlippedX(false);
            break;
        case Direction::kRight:
            this->setRotation(0);
            this->setFlippedX(true);
            break;
        default:
            break;
    }
}

}  // namespace th_valley
