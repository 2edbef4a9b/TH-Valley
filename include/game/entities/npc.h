#ifndef NPC_H_
#define NPC_H_

#include "game/bag.h"
#include "game/character.h"

namespace th_valley {

class NPC : public Character {
public:
    NPC();
    ~NPC() override = default;

    // --- NPC Specific Functions ---

    // Show dialogue box
    void ShowTalkBox(cocos2d::Node* parent, Bag* bag);

    // NPC behavior AI (e.g. go to square in day, return home at night)
    void UpdateAI(float dt);

    // Whether interaction is allowed
    bool CanInteract() const { return true; }

protected:
    // Dialogue content data
    std::vector<std::string> dialogues_;
};

}  // namespace th_valley

#endif  // NPC_H_
