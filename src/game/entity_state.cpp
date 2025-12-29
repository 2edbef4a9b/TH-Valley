#include "game/entity_state.h"
#include "game/entity.h" // Need to include Entity's complete definition to call its methods - Refactored with State Pattern

namespace th_valley {

// ================== IdleState (Standby) - Refactored with State Pattern ==================

void IdleState::Enter(Entity* entity) {
    // Play standby animation - Refactored with State Pattern
    entity->stopAllActions();
    cocos2d::log("State: Enter Idle");
}

void IdleState::Execute(Entity* entity, float delta) {
    // Slightly recover energy when on standby (example logic) - Refactored with State Pattern
    double currentEnergy = entity->GetData().GetEnergy();
    if (currentEnergy < 100.0) {
        entity->GetData().SetEnergy(currentEnergy + delta * 2.0);
    }
}

void IdleState::Exit(Entity* entity) {
    cocos2d::log("State: Exit Idle");
}

void IdleState::HandleInput(Entity* entity, const Input& input) {
    // If pressing any WASD key, switch to move state - Refactored with State Pattern
    if (input.isKeyPressed) {
        entity->ChangeState(new MoveState());
    }
    // If clicking mouse, switch to attack state - Refactored with State Pattern
    else if (input.isClick) {
        entity->ChangeState(new ActionState());
    }
}

// ================== MoveState (Movement) - Refactored with State Pattern ==================

void MoveState::Enter(Entity* entity) {
    cocos2d::log("State: Enter Move");
}

void MoveState::Execute(Entity* entity, float delta) {
    // Get current direction and move coordinates - Refactored with State Pattern
    cocos2d::Vec2 offset(0, 0);
    float speed = 100.0f; // 100 pixels/sec - Refactored with State Pattern

    switch (entity->GetDirection()) {
        case Entity::Direction::kUp:    offset.y = 1; break;
        case Entity::Direction::kDown:  offset.y = -1; break;
        case Entity::Direction::kLeft:  offset.x = -1; break;
        case Entity::Direction::kRight: offset.x = 1; break;
    }

    // Actually modify Entity coordinates - Refactored with State Pattern
    cocos2d::Vec2 newPos = entity->getPosition() + (offset * speed * delta);
    entity->setPosition(newPos);

    // Movement consumes hunger - Refactored with State Pattern
    double hunger = entity->GetData().GetHunger();
    if (hunger > 0) {
        entity->GetData().SetHunger(hunger - delta * 1.0);
    } else {
        // Too hungry to move, switch back to Idle - Refactored with State Pattern
        entity->ChangeState(new IdleState());
    }
}

void MoveState::Exit(Entity* entity) {
    cocos2d::log("State: Exit Move");
}

void MoveState::HandleInput(Entity* entity, const Input& input) {
    // If releasing key, switch back to Idle - Refactored with State Pattern
    if (!input.isKeyPressed) {
        entity->ChangeState(new IdleState());
    }
    // Can also attack while moving - Refactored with State Pattern
    else if (input.isClick) {
        entity->ChangeState(new ActionState());
    }
}

// ================== ActionState (Action/Attack) - Refactored with State Pattern ==================

ActionState::ActionState() : duration_timer_(0.0f) {}

void ActionState::Enter(Entity* entity) {
    cocos2d::log("State: Enter Action");
    duration_timer_ = 0.0f;
    // For example: entity->PlayAnimation("attack"); - Refactored with State Pattern
}

void ActionState::Execute(Entity* entity, float delta) {
    // Action state typically automatically ends when "animation finishes" or "time is up" - Refactored with State Pattern
    duration_timer_ += delta;

    if (duration_timer_ >= ACTION_DURATION) {
        // Action ends, automatically switch back to standby - Refactored with State Pattern
        entity->ChangeState(new IdleState());
    }
}

void ActionState::Exit(Entity* entity) {
    cocos2d::log("State: Exit Action");
}

void ActionState::HandleInput(Entity* entity, const Input& input) {
    // During action stiffness, typically doesn't accept movement input, so leave this blank - Refactored with State Pattern
    // This is "stiffness" - Refactored with State Pattern
}

} // namespace th_valley