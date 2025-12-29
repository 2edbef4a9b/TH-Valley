#include "game/entity.h"

namespace th_valley {

Entity::Entity() {
    // When constructing, initial state is empty, initialize in init - Refactored with State Pattern
    current_state_ = nullptr;
}

Entity::~Entity() {
    // Must clean up current state memory - Refactored with State Pattern
    if (current_state_) {
        current_state_->Exit(this);
        delete current_state_;
        current_state_ = nullptr;
    }
}

bool Entity::init() {
    if (!Sprite::init()) {
        return false;
    }

    // Enable Update scheduling - Refactored with State Pattern
    this->scheduleUpdate();

    // Initialize default state to Idle - Refactored with State Pattern
    ChangeState(new IdleState());

    return true;
}

void Entity::InitEntity(cocos2d::Node* parent) {
    if (parent) {
        parent->addChild(this);
        this->setPosition(parent->getContentSize() / 2);
    }
}

void Entity::update(float delta) {
    // Core: delegate to current state for execution - Refactored with State Pattern
    if (current_state_) {
        current_state_->Execute(this, delta);
    }
}

void Entity::ChangeState(IEntityState* newState) {
    // 1. Exit old state - Refactored with State Pattern
    if (current_state_) {
        current_state_->Exit(this);
        delete current_state_; // Release old state memory - Refactored with State Pattern
    }

    // 2. Switch pointer - Refactored with State Pattern
    current_state_ = newState;

    // 3. Enter new state - Refactored with State Pattern
    if (current_state_) {
        current_state_->Enter(this);
    }
}

void Entity::OnInput(const Input& input) {
    // Pass input to current state for processing - Refactored with State Pattern
    if (current_state_) {
        current_state_->HandleInput(this, input);
    }
}

// --- Getter / Setter - Refactored with State Pattern ---

void Entity::SetDirection(Direction direction) {
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

Entity::Direction Entity::GetDirection() const {
    return direction_;
}

EntityData& Entity::GetData() {
    return entity_data_;
}

const EntityData& Entity::GetData() const {
    return entity_data_;
}

} // namespace th_valley
