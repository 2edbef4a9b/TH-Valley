#include"avatar/illustratedguide.h"
#include"cocos2d.h"
#include <unordered_map>
Illustratedguide::Illustratedguide():totalnum(5) {}

void Illustratedguide::loadguide(){
    guide = Getguide();
}

 std::unordered_map<std::string, std::string> Illustratedguide::Getguide(){
    static const std::unordered_map<std::string, std::string> guidelist(
        {{"Axe", "Cuts trees and wood"},
         {"Hoe", "Loosens soil and weeds"},
         {"Seed", "Reproduces plants"},
         {"Fruit", "Mature product of plants."},
         {"Gift", "Item expressing feelings."},
         {"Animal", "living organisms that move, eat, and reproduce"}});
    return guidelist;
}

std::string Illustratedguide::read(std::string key){

    auto pos = guide.find(key);
    
    if (pos != guide.end()) {
        return guide[key];
    }
    else {
        return "unfind";

    }
}

void Illustratedguide::showdesignated(std::string key) {

    auto pos = guide.find(key);

    if (pos != guide.end()) {
        CCLOG("%s | %s", key, guide[key]);
    } else {
        CCLOG("unfind");
    }
}

void Illustratedguide::showtotal() {

    for (const auto& [key, value] : guide) {
        CCLOG("%s | %s",key,value);
    }
}
