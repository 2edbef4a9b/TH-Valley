#include <string>
#include <unordered_map>
#include"cocos2d.h"
#include"creature/creature.h"
#include <random>
#include"avatar/npc.h"
#include<math.h>


NPC::NPC() {






}


float NPC::trading(wave day, quilitylist quility) {









    return price(day, quility);
}
float NPC::buying(wave day, quilitylist quility,float giving) {
    float price = price(wave day, quilitylist quility);
    return giving - price;
}




void NPC::conversation(cocos2d::Sprite* character) {












}
float NPC::price(wave day, quilitylist quility) {
    std::random_device rd;   // 获取随机数种子
    std::mt19937 gen(rd());  // 使用梅森旋转算法生成随机数
    std::uniform_int_distribution<> dis(1, 100);  // 定义随机数范围为1到100
    float wavepoint = dis / 100; 
    float price = wavepoint * day + quility;
    return price;
}
