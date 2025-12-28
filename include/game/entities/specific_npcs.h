#ifndef SPECIFIC_NPCS_H_
#define SPECIFIC_NPCS_H_

#include "game/npc.h"

namespace th_valley {

class Sandy : public NPC {
public:
    Sandy() {
        SetName("Sandy");
        InitCharacterAnim("assets/avatar/sandy.png");
        dialogues_ = {"Hello!", "Nice weather today."};
    }
};

class Abigail : public NPC {
public:
    Abigail() {
        SetName("Abigail");
        InitCharacterAnim("assets/avatar/Abigail.png");
        dialogues_ = {"hi!", "Bad weather today."};
    }
};
} // namespace th_valley

#endif // SPECIFIC_NPCS_H_