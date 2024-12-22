#include "game/entity.h"

namespace th_valley {

cocos2d::Sprite* Entity::GetSprite() const { return sprite_; }

void Entity::SetDirection(Direction direction) {
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

void Entity::SetSprite(cocos2d::Sprite* sprite) { sprite_ = sprite; }

void Entity::SetPosition(const cocos2d::Vec2& position) {
    position_ = position;
    sprite_->setPosition(position_);
}

}  // namespace th_valley
