#include "game/entity.h"

namespace th_valley {

void Entity::SetState(EntityState state) { state_ = state; }

Entity::EntityState Entity::GetState() const { return state_; }

void Entity::SetDirection(Direction direction) { direction_ = direction; }

Entity::Direction Entity::GetDirection() const { return direction_; }

void Entity::InitEntity(cocos2d::Node* parent) {
    auto* texture =
        cocos2d::Director::getInstance()->getTextureCache()->addImage(
            "assets/tilesheets/koishi.png");

    cocos2d::Rect frame_rect(0, 0, 3072, 3072);
    auto* sprite_frame =
        cocos2d::SpriteFrame::createWithTexture(texture, frame_rect);
    sprite_ = cocos2d::Sprite::createWithSpriteFrame(sprite_frame);
    sprite_->setScale(32.0F / 3072.0F);
    sprite_->setAnchorPoint(cocos2d::Vec2(0.5f, 0.0f));
    parent->addChild(sprite_, 2);
}

void Entity::ChangeDirection(Direction direction) {
    direction_ = direction;
    switch (direction_) {
        case Direction::kUp:
            sprite_->setRotation(0);
            sprite_->setFlippedX(false);
            break;
        case Direction::kDown:
            sprite_->setRotation(0);
            sprite_->setFlippedX(false);
            break;
        case Direction::kLeft:
            sprite_->setRotation(0);
            sprite_->setFlippedX(false);
            break;
        case Direction::kRight:
            sprite_->setRotation(0);
            sprite_->setFlippedX(true);
            break;
        default:
            break;
    }
}

void Entity::SetPosition(const cocos2d::Vec2& position) {
    position_ = position;
    sprite_->setPosition(position_);
}

cocos2d::Sprite* Entity::GetSprite() const { return sprite_; }

void Entity::SetSprite(cocos2d::Sprite* sprite) { sprite_ = sprite; }

}  // namespace th_valley
