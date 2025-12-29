#ifndef AVATAR_H_
#define AVATAR_H_

#include "game/character.h" 
#include "game/tool_strategies.h"
#include <list>

namespace th_valley {

class Avatar : public Character {
public:
    Avatar();
    ~Avatar() override;

    bool init() override;
    void update(float delta) override;

    // --- Strategy Pattern Implementation - Refactored with Strategy Pattern ---
    void SetToolStrategy(IToolStrategy* newStrategy);
    void UseCurrentTool(cocos2d::Vec2 targetPos);

    // --- Input handling specific logic - Refactored with Strategy Pattern ---
    // Responds to keyboard/mouse input - Refactored with Strategy Pattern
    void HandleInput(); 

private:
    IToolStrategy* currentToolStrategy_ = nullptr;
    
    // Player specific attributes like profession, weapons, etc. - Refactored with Strategy Pattern
    std::string profession;
};

} // namespace th_valley

#endif // AVATAR_H_