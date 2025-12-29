#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "game/entity.h"
#include <string>
#include <vector>

namespace th_valley {

// Character is responsible for handling common "human" logic: name, animation, direction - Refactored with Strategy Pattern
class Character : public Entity {
public:
    Character();
    ~Character() override = default;

    // Initialize character common animations (load from texture atlas) - Refactored with Strategy Pattern
    // textureName: image path (e.g. "assets/avatar/sandy.png") - Refactored with Strategy Pattern
    void InitCharacterAnim(const std::string& textureName);

    // Set/get name - Refactored with Strategy Pattern
    void SetName(const std::string& name) { name_ = name; }
    std::string GetName() const { return name_; }

    // Common movement rendering (play animation based on Entity's direction_) - Refactored with Strategy Pattern
    virtual void UpdateAnimationState(); 

protected:
    std::string name_;
    
    // Store animation frames for 4 directions - Refactored with Strategy Pattern
    // 0:Down, 1:Right, 2:Up, 3:Left - Refactored with Strategy Pattern
    cocos2d::Vector<cocos2d::SpriteFrame*> walk_frames_[4];
    
    // Currently playing action - Refactored with Strategy Pattern
    cocos2d::Action* current_action_ = nullptr;
};

} // namespace th_valley

#endif // CHARACTER_H_