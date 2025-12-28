#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "game/entity.h"
#include <string>
#include <vector>

namespace th_valley {

// Character 负责处理通用的“人”的逻辑：名字、动画、朝向
class Character : public Entity {
public:
    Character();
    ~Character() override = default;

    // 初始化角色通用动画 (从纹理集加载)
    // textureName: 图片路径 (e.g. "assets/avatar/sandy.png")
    void InitCharacterAnim(const std::string& textureName);

    // 设置/获取名字
    void SetName(const std::string& name) { name_ = name; }
    std::string GetName() const { return name_; }

    // 通用移动渲染 (根据 Entity 的 direction_ 播放动画)
    virtual void UpdateAnimationState(); 

protected:
    std::string name_;
    
    // 存储4个方向的动画帧
    // 0:Down, 1:Right, 2:Up, 3:Left
    cocos2d::Vector<cocos2d::SpriteFrame*> walk_frames_[4];
    
    // 当前播放的动作
    cocos2d::Action* current_action_ = nullptr;
};

} // namespace th_valley

#endif // CHARACTER_H_