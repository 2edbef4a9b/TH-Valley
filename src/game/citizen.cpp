#include "game/citizen.h"

#include <cmath>
#include <string>

#include "2d/CCNode.h"
#include "frontend/talk_box.h"

void Citizen::ShowTalkBox(cocos2d::Node* parent) {
    auto* talk_box = TalkBox::createWithEntries(
        TalkBox::inputJson("assets/dialogue/Sebastian.json"), "Sebastian",
        "assets/Portraits/Sebastian.png");
    parent->addChild(talk_box, 100);
}
