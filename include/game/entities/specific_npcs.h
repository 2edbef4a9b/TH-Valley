#ifndef SPECIFIC_NPCS_H_
#define SPECIFIC_NPCS_H_

#include "game/npc.h"

namespace th_valley {

class Sandy : public NPC {
public:
    Sandy() {
        SetName("Sandy");
        InitCharacterAnim("assets/avatar/sandy.png"); // 使用父类方法加载资源
        
        // 初始化 Sandy 特有的对话
        dialogues_ = {"Hello!", "Nice weather today."};
    }
};

class Abigail : public NPC {
public:
    Abigail() {
        SetName("Abigail");
        InitCharacterAnim("assets/avatar/Abigail.png");
    }
};

// ... 其他 NPC ...

} // namespace th_valley

#endif // SPECIFIC_NPCS_H_