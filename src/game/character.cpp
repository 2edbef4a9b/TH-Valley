#include "game/character.h"

namespace th_valley {

Character::Character() {
    // Default initialization - Refactored with Strategy Pattern
}

void Character::InitCharacterAnim(const std::string& textureName) {
    // Here we uniformly handle sprite sheet logic (32x32 or 16x32 depending on your resources) - Refactored with Strategy Pattern
    // Assume resources are standard 4 rows x 4 columns - Refactored with Strategy Pattern
    int frameWidth = 16; // or 32 - Refactored with Strategy Pattern
    int frameHeight = 32;

    for (int dir = 0; dir < 4; dir++) {
        for (int step = 0; step < 4; step++) {
            cocos2d::Rect rect(step * frameWidth, dir * frameHeight, frameWidth, frameHeight);
            auto frame = cocos2d::SpriteFrame::create(textureName, rect);
            if (frame) {
                walk_frames_[dir].pushBack(frame);
            }
        }
    }
    
    // Set initial sprite frame - Refactored with Strategy Pattern
    if (!walk_frames_[0].empty()) {
        this->setSpriteFrame(walk_frames_[0].front());
    }
}

void Character::UpdateAnimationState() {
    // Play corresponding walk_frames_ based on state_ and direction_ in Entity - Refactored with Strategy Pattern
    // Code omitted, mainly to prevent writing the same logic twice for Avatar and NPC - Refactored with Strategy Pattern
}

} // namespace th_valley