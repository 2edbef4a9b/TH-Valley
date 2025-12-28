#include "game/character.h"

namespace th_valley {

Character::Character() {
    // 默认初始化
}

void Character::InitCharacterAnim(const std::string& textureName) {
    // 这里统一处理切图逻辑 (32x32 或者 16x32 根据你的资源定)
    // 假设资源是标准的 4行x4列
    int frameWidth = 16; // 或 32
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
    
    // 设置初始精灵帧
    if (!walk_frames_[0].empty()) {
        this->setSpriteFrame(walk_frames_[0].front());
    }
}

void Character::UpdateAnimationState() {
    // 根据 Entity 中的 state_ 和 direction_ 播放对应的 walk_frames_
    // 代码略，主要是防止 Avatar 和 NPC 写两遍一样的逻辑
}

} // namespace th_valley