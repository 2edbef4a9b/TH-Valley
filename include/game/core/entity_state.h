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

// --- Interface - Refactored with State Pattern ---
class IEntityState {
public:
    virtual ~IEntityState() = default;
    virtual void Enter(Entity* entity) = 0;
    virtual void Execute(Entity* entity, float delta) = 0;
    virtual void Exit(Entity* entity) = 0;
    virtual void HandleInput(Entity* entity, const Input& input) = 0;
};

// --- 1. Standby State (Idle) - Refactored with State Pattern ---
class IdleState : public IEntityState {
public:
    void Enter(Entity* entity) override;
    void Execute(Entity* entity, float delta) override;
    void Exit(Entity* entity) override;
    void HandleInput(Entity* entity, const Input& input) override;
};

// --- 2. Movement State (Move) - Refactored with State Pattern ---
class MoveState : public IEntityState {
public:
    void Enter(Entity* entity) override;
    void Execute(Entity* entity, float delta) override;
    void Exit(Entity* entity) override;
    void HandleInput(Entity* entity, const Input& input) override;
};

// --- 3. Action/Attack State (Action) - Refactored with State Pattern ---
class ActionState : public IEntityState {
private:
    float duration_timer_; // Record action duration - Refactored with State Pattern
    const float ACTION_DURATION = 0.5f; // Action duration 0.5 seconds - Refactored with State Pattern

public:
    ActionState();
    void Enter(Entity* entity) override;
    void Execute(Entity* entity, float delta) override;
    void Exit(Entity* entity) override;
    void HandleInput(Entity* entity, const Input& input) override;
};

} // namespace th_valley

#endif // ENTITY_STATE_H_