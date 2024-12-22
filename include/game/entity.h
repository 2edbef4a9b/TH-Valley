#ifndef ENTITY_H_
#define ENTITY_H_

#include <string_view>

#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "cocos2d.h"
#include "game/entity_data.h"
#include "math/Vec2.h"

namespace th_valley {

class Entity : public cocos2d::Sprite {
public:
    enum class Direction {
        kUp,
        kDown,
        kLeft,
        kRight,
    };

    enum class EntityState {
        kIdle,
        kMove,
        kAction,
        kDead,
    };

    Entity() = default;
    ~Entity() override = default;

    Entity(const Entity& other) = delete;
    Entity& operator=(const Entity& other) = delete;
    Entity(Entity&& other) = delete;
    Entity& operator=(Entity&& other) = delete;

    void SetState(EntityState state);
    EntityState GetState() const;
    void SetDirection(Direction direction);
    Direction GetDirection() const;

    bool init() override;
    void update(float delta) override;

    virtual void InitEntity(cocos2d::Node* parent);
    virtual void ChangeDirection(Direction direction);

private:
    cocos2d::Vec2 position_;
    EntityData entity_data_;
    EntityState state_{EntityState::kIdle};
    Direction direction_{Direction::kDown};
};

}  // namespace th_valley

#endif  // ENTITY_H_
