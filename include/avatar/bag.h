#ifndef BAG_H_
#define BAG_H_
#include "cocos2d.h"
#include <unordered_map>
#include <vector>
#include <string>

enum quilitylist { high = 1000, low = 500, lowest = 200, perfect = 10000 };

struct ItemSprite {
    // ��Ʒ����
    std::string name;
    // ��Ʒ����
    int quantity;
    // Ʒ��ö��
    quilitylist qulity;
    // ͼƬչʾ 
    cocos2d::Sprite* sprite;
    // ���캯��
    ItemSprite(std::string name, int quantity, quilitylist qulity,cocos2d::Sprite* sprite)
        : name(name), quantity(quantity), qulity(qulity), sprite(sprite) {}
};

class Bag {
public:
    bool isempty() const;
    bool isfull() const;
    void addobj(std::string key,  ItemSprite* additem);
    ItemSprite* myfind(std::string key);
    void reduce(std::string key);
    Bag();
    void BagInit();


private:

    std::unordered_map<std::string, ItemSprite*> bag;
    std::unordered_map<std::string, int> idMap;
    bool isopen = false;
    int currentnum;
    int maxnum;
};
#endif
