#include "game/entity.h"

namespace th_valley {

Entity::Entity() {
    // 构造时，初始状态为空，init 中再初始化
    current_state_ = nullptr;
}

Entity::~Entity() {
    // 必须清理当前状态内存
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

    // 开启 Update 调度
    this->scheduleUpdate();

    // 初始化默认状态为 Idle
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
    // 核心：委托给当前状态执行
    if (current_state_) {
        current_state_->Execute(this, delta);
    }
}

void Entity::ChangeState(IEntityState* newState) {
    // 1. 退出旧状态
    if (current_state_) {
        current_state_->Exit(this);
        delete current_state_; // 释放旧状态内存
    }

    // 2. 切换指针
    current_state_ = newState;

    // 3. 进入新状态
    if (current_state_) {
        current_state_->Enter(this);
    }
}

void Entity::OnInput(const Input& input) {
    // 将输入传递给当前状态处理
    if (current_state_) {
        current_state_->HandleInput(this, input);
    }
}

// --- Getter / Setter ---

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
