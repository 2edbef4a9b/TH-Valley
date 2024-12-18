#include <string>
#include <unordered_map>
#include"cocos2d.h"
#include"creature/creature.h"
#include <random>
#include"avatar/npc.h"
#include<math.h>

int NPC::data(wave day) {
    int data = 0;
    if (day == Mon) {
        data = 0;
    }
    else if (day == Tues) {
        data = 1;
    }
    else if (day == Wens) {
        data = 2;
    }
    else if (day == Thurs) {
        data = 3;
    }
    else if (day == Fri) {
        data = 4;
    }
    else if (day == Sat) {
        data = 5;
    }
    else if (day ==Sun) {
        data = 6;
    }
    return data;
}
NPC::NPC() {
    NPCbag.addobj(
        "Axe",
        &ItemSprite("Axe", 10, perfect,
                    cocos2d::Sprite::create(
                        "avatar/tool.png",
                        cocos2d::Rect(0 + 5 * 16, 32 + 4 * 32, 16, 16))));
    NPCbag.addobj(
        "weedA",
        &ItemSprite("weedA", 400, low,
                    cocos2d::Sprite::create(
                        "avatar/tool.png",
                        cocos2d::Rect(0 + 5 * 16, 32 + 4 * 32, 16, 16))));
    NPCbag.addobj(
        "Hoe",
        &ItemSprite("Hoe", 8, high,
                    cocos2d::Sprite::create(
                        "avatar/tool.png",
                        cocos2d::Rect(0 + 5 * 16, 32 + 4 * 32, 16, 16))));
    NPCbag.addobj(
        "strewberry",
        &ItemSprite("strewberry", 100, low,
                    cocos2d::Sprite::create(
                        "avatar/tool.png",
                        cocos2d::Rect(0 + 5 * 16, 32 + 4 * 32, 16, 16))));
    NPCbag.addobj(
        "fishingrod",
        &ItemSprite("fishingrod", 1000, lowest,
                    cocos2d::Sprite::create(
                        "avatar/tool.png",
                        cocos2d::Rect(0 + 5 * 16, 32 + 4 * 32, 16, 16))));
    NPCbag.addobj(
        "weedB",
        &ItemSprite("weedB", 400, low,
                    cocos2d::Sprite::create(
                        "avatar/tool.png",
                        cocos2d::Rect(0 + 5 * 16, 32 + 4 * 32, 16, 16))));

    market[0].insert(std::make_pair("Axe", price(Mon, "Axe")));

    market[1].insert(std::make_pair("weedA", price(Tues, "weedA")));

    market[2].insert(std::make_pair("Hoe", price(Wens, "Hoe")));

    market[3].insert(std::make_pair("strewberry", price(Thurs, "strewberry")));

    market[4].insert(std::make_pair("fishingrod", price(Fri, "fishingrod")));

    market[5].insert(std::make_pair("Hoe", price(Sat, "Hoe")));

    market[6].insert(std::make_pair("weedB", price(Sun, "weedB")));
}

float NPC::buying(wave day, std::string good, float giving) {

    auto pos = market[data(day)].find(good);
    if (pos != market[data(day)].end()) {
        if (NPCbag.myfind(good)!=NULL&&(giving - price(day, good)) >= 0) {
            CCLOG("buy successfully");
            return giving - price(day, good);
        } else if((giving - price(day, good)) < 0) {
            CCLOG("buy unsuccessfully : insufficient money");
        } else {
            CCLOG("buy unsuccessfully : insufficient good");
        }
    }
    else {
        CCLOG("buy unsuccessfully : sold out");
        
    }
    return -1.0f;
}




void NPC::showgoods() {
}

float NPC::price(wave day, std::string good) {
    std::random_device rd;   // 获取随机数种子
    std::mt19937 gen(rd());  // 使用梅森旋转算法生成随机数
    std::uniform_int_distribution<> dis(1, 100);  // 定义随机数范围为1到100
    float wavepoint = dis(gen) /100; 
    float price = wavepoint * day + (NPCbag.myfind(good))->quantity;
    return price;
}
