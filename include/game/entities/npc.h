#ifndef NPC_H_
#define NPC_H_

#include "game/character.h" // 继承 Character
#include "game/bag.h"

namespace th_valley {

class NPC : public Character {
public:
    NPC();
    ~NPC() override = default;

    // --- NPC 特有功能 ---
    
    // 显示对话框
    void ShowTalkBox(cocos2d::Node *parent, Bag* bag);
    
    // NPC 行为 AI (比如白天去广场，晚上回家)
    void UpdateAI(float dt);

    // 是否允许交互
    bool CanInteract() const { return true; }

protected:
    // 对话内容数据
    std::vector<std::string> dialogues_;
};

} // namespace th_valley

#endif // NPC_H_