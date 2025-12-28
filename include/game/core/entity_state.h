#ifndef ENTITY_STATE_H_
#define ENTITY_STATE_H_

#include "cocos2d.h"

namespace th_valley {

class Entity; 

struct Input {
    bool isKeyPressed;
    cocos2d::EventKeyboard::KeyCode keyCode;
    bool isClick;
    cocos2d::Vec2 clickPosition;
};

// --- 接口 Interface ---
class IEntityState {
public:
    virtual ~IEntityState() = default;
    virtual void Enter(Entity* entity) = 0;
    virtual void Execute(Entity* entity, float delta) = 0;
    virtual void Exit(Entity* entity) = 0;
    virtual void HandleInput(Entity* entity, const Input& input) = 0;
};

// --- 1. 待机状态 (Idle) ---
class IdleState : public IEntityState {
public:
    void Enter(Entity* entity) override;
    void Execute(Entity* entity, float delta) override;
    void Exit(Entity* entity) override;
    void HandleInput(Entity* entity, const Input& input) override;
};

// --- 2. 移动状态 (Move) ---
class MoveState : public IEntityState {
public:
    void Enter(Entity* entity) override;
    void Execute(Entity* entity, float delta) override;
    void Exit(Entity* entity) override;
    void HandleInput(Entity* entity, const Input& input) override;
};

// --- 3. 动作/攻击状态 (Action) ---
class ActionState : public IEntityState {
private:
    float duration_timer_; // 记录动作持续时间
    const float ACTION_DURATION = 0.5f; // 动作持续0.5秒

public:
    ActionState();
    void Enter(Entity* entity) override;
    void Execute(Entity* entity, float delta) override;
    void Exit(Entity* entity) override;
    void HandleInput(Entity* entity, const Input& input) override;
};

} // namespace th_valley

#endif // ENTITY_STATE_H_