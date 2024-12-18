#ifndef NPC_H_
#define NPC_H_
#include <string>
#include <unordered_map>
#include"cocos2d.h"
#include"creature/creature.h"
#include"avatar/bag.h"
#include"avatar/illustratedguide.h"


enum wave {
    Mon = 10,
    Tues = -10,
    Wens = 20,
    Thurs = -5,
    Fri = -20,
    Sat = 50,
    Sun = 60
};
class NPC : public Creature{

public:
    NPC();
    float price(wave day, std::string good);
    void showgoods();
    float buying(wave day, std::string good, float giving);
    int data(wave day);

private :
    std::unordered_map<std::string, float> *market;
    Bag NPCbag;
};

#endif  // NPC_H_
