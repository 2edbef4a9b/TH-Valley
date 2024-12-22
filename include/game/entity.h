#ifndef ENTITY_H_
#define ENTITY_H_

#include "2d/CCSprite.h"
#include "cocos2d.h"
#include "game/entity_data.h"
#include "math/Vec2.h"

namespace th_valley {

class Entity : public cocos2d::Sprite {
public:
    Entity() = default;
    ~Entity() override = default;

    Entity(const Entity& other) = delete;
    Entity& operator=(const Entity& other) = delete;
    Entity(Entity&& other) = delete;
    Entity& operator=(Entity&& other) = delete;

private:
    enum class EntityState {
        kIdle,
        kMove,
        kAction,
        kDead,
    };

    void update(float delta_time) override;

    cocos2d::Vec2 position_;
    cocos2d::Vec2 velocity_;
    cocos2d::Sprite* sprite_{};
    EntityData entity_data_;
    EntityState state_{EntityState::kIdle};
};

}  // namespace th_valley

#endif  // ENTITY_H_
