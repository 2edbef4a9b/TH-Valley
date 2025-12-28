#ifndef ENTITY_H_
#define ENTITY_H_

#include "cocos2d.h"
#include "game/entity_data.h"
#include "game/entity_state.h" 

namespace th_valley {

class Entity : public cocos2d::Sprite {
public:
    enum class Direction {
        kDown, kRight, kUp, kLeft,
    };

    Entity();
    ~Entity() override;

    // 禁用拷贝，防止状态指针被浅拷贝导致重复释放
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    bool init() override;
    void update(float delta) override;

    // --- 状态模式方法 ---
    void ChangeState(IEntityState* newState);
    
    // 输入处理入口（供外部层调用)
    void OnInput(const Input& input);

    // --- 供状态类调用的接口 ---
    void SetDirection(Direction direction);
    Direction GetDirection() const;
    EntityData& GetData(); // 允许修改数据
    const EntityData& GetData() const; // 只读数据

    virtual void InitEntity(cocos2d::Node* parent);

private:
    EntityData entity_data_;
    Direction direction_{Direction::kDown};
    
    // 当前状态指针
    IEntityState* current_state_ = nullptr;
};

}  // namespace th_valley

#endif  // ENTITY_H_