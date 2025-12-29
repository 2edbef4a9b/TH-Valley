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

    // Disable copying to prevent shallow copy of state pointer causing double free - Refactored with State Pattern
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    bool init() override;
    void update(float delta) override;

    // --- State Pattern methods - Refactored with State Pattern ---
    void ChangeState(IEntityState* newState);

    // Input processing entry (for external layer to call) - Refactored with State Pattern
    void OnInput(const Input& input);

    // --- Interfaces for state classes to call - Refactored with State Pattern ---
    void SetDirection(Direction direction);
    Direction GetDirection() const;
    EntityData& GetData(); // Allow data modification - Refactored with State Pattern
    const EntityData& GetData() const; // Read-only data - Refactored with State Pattern

    virtual void InitEntity(cocos2d::Node* parent);

private:
    EntityData entity_data_;
    Direction direction_{Direction::kDown};
    
    // Current state pointer - Refactored with State Pattern
    IEntityState* current_state_ = nullptr;
};

}  // namespace th_valley

#endif  // ENTITY_H_