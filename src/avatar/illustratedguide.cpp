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
         {"StrawberrySeed", "Reproduces Strawberriess"},
         {"PotatoSeed", "Reproduces Potatoes"},
         {"CarrotSeed", "Reproduces Carrots"},
         {"Strawberry", "Mature product of Strawberrys."},
         {"Potato", "Mature product of Potatos."},
         {"Carrot", "Mature product of Carrots."},
         {"Gift", "Item expressing feelings."},
         {"Pig", "animal sample1"},
         {"Chicken", "IKUN"}});
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
