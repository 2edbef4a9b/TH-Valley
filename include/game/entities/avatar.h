#ifndef AVATAR_H_
#define AVATAR_H_

#include "game/character.h" // 继承 Character
#include "game/tool_strategies.h"

namespace th_valley {

class Avatar : public Character {
public:
    Avatar();
    ~Avatar() override;

    bool init() override;
    void update(float delta) override;

    // --- 策略模式 (保留) ---
    void SetToolStrategy(IToolStrategy* newStrategy);
    void UseCurrentTool(cocos2d::Vec2 targetPos);
    
    // --- 输入处理特有逻辑 ---
    // 响应键盘/鼠标
    void HandleInput(); 

private:
    IToolStrategy* currentToolStrategy_ = nullptr;
    
    // 职业、武器等玩家特有属性
    std::string profession;
};

} // namespace th_valley

#endif // AVATAR_H_