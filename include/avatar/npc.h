#ifndef NPC_H_
#define NPC_H_
#include <string>
#include <unordered_map>
#include"cocos2d.h"
#include"creature/creature.h"
#include"avatar/bag.h"
#include"avatar/illustratedguide.h"

class NPC : public Creature{

public:
    NPC();
    float trading(wave day, quilitylist quility);
    void conversation(cocos2d::Sprite *character);
    float buying(wave day, quilitylist quility,float giving);

private :
    std::unordered_map<std::string, int> market;
    enum quilitylist { high = 1000, low = 500, lowest = 200, perfect = 10000 };
    enum wave { Mon = 10, Tues = -10, Wednes = 20, Thurs = -5 ,Fri = -20,Sat = 50,sun = 60};
    float price(wave day, quilitylist quility);
    Bag NPCbag;
    Illustratedguide sellinggood;
};

#endif  // NPC_H_
