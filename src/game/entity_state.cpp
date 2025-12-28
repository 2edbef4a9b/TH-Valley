#include "game/entity_state.h"
#include "game/entity.h" // 需要包含 Entity 的完整定义以调用其方法

namespace th_valley {

// ================== IdleState (待机) ==================

void IdleState::Enter(Entity* entity) {
    // 播放待机动画
    entity->stopAllActions();
    cocos2d::log("State: Enter Idle");
}

void IdleState::Execute(Entity* entity, float delta) {
    // 待机时稍微回复一点体力 (示例逻辑)
    double currentEnergy = entity->GetData().GetEnergy();
    if (currentEnergy < 100.0) {
        entity->GetData().SetEnergy(currentEnergy + delta * 2.0);
    }
}

void IdleState::Exit(Entity* entity) {
    cocos2d::log("State: Exit Idle");
}

void IdleState::HandleInput(Entity* entity, const Input& input) {
    // 如果按下 WASD 任意键，切换到移动状态
    if (input.isKeyPressed) {
        entity->ChangeState(new MoveState()); 
    }
    // 如果点击鼠标，切换到攻击状态
    else if (input.isClick) {
        entity->ChangeState(new ActionState());
    }
}

// ================== MoveState (移动) ==================

void MoveState::Enter(Entity* entity) {
    cocos2d::log("State: Enter Move");
}

void MoveState::Execute(Entity* entity, float delta) {
    // 获取当前朝向并移动坐标
    cocos2d::Vec2 offset(0, 0);
    float speed = 100.0f; // 100像素/秒

    switch (entity->GetDirection()) {
        case Entity::Direction::kUp:    offset.y = 1; break;
        case Entity::Direction::kDown:  offset.y = -1; break;
        case Entity::Direction::kLeft:  offset.x = -1; break;
        case Entity::Direction::kRight: offset.x = 1; break;
    }

    // 实际修改 Entity 坐标
    cocos2d::Vec2 newPos = entity->getPosition() + (offset * speed * delta);
    entity->setPosition(newPos);

    // 移动消耗饥饿度
    double hunger = entity->GetData().GetHunger();
    if (hunger > 0) {
        entity->GetData().SetHunger(hunger - delta * 1.0);
    } else {
        // 饿了走不动，切回 Idle
        entity->ChangeState(new IdleState());
    }
}

void MoveState::Exit(Entity* entity) {
    cocos2d::log("State: Exit Move");
}

void MoveState::HandleInput(Entity* entity, const Input& input) {
    // 如果松开按键，切回 Idle
    if (!input.isKeyPressed) {
        entity->ChangeState(new IdleState());
    }
    // 移动中也可以攻击
    else if (input.isClick) {
        entity->ChangeState(new ActionState());
    }
}

// ================== ActionState (动作/攻击) ==================

ActionState::ActionState() : duration_timer_(0.0f) {}

void ActionState::Enter(Entity* entity) {
    cocos2d::log("State: Enter Action");
    duration_timer_ = 0.0f;
    // 比如：entity->PlayAnimation("attack");
}

void ActionState::Execute(Entity* entity, float delta) {
    // 动作状态通常是“播放完动画”或者“时间到了”就自动结束
    duration_timer_ += delta;

    if (duration_timer_ >= ACTION_DURATION) {
        // 动作结束，自动切回待机
        entity->ChangeState(new IdleState());
    }
}

void ActionState::Exit(Entity* entity) {
    cocos2d::log("State: Exit Action");
}

void ActionState::HandleInput(Entity* entity, const Input& input) {
    // 动作硬直期间，通常不接受移动输入，所以这里留空
    // 也就是“硬直”
}

} // namespace th_valley