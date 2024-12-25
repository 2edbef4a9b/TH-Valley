#include "game/citizen.h"

#include <cmath>
#include <string>

#include "2d/CCNode.h"
#include "frontend/talk_box.h"

void Citizen::ShowTalkBox(cocos2d::Node* parent, Bag* bag_, std::string name) {
    auto* talk_box = TalkBox::createWithEntries(
        TalkBox::inputJson("assets/dialogue/Sebastian.json"), "Sebastian",
        "assets/Portraits/Sebastian.png");
    talk_box->getBag(bag_);
    talk_box->getName(name);
    parent->addChild(talk_box, 100);
}
