#ifndef BAG_H_
#define BAG_H_
#include <string>
#include <unordered_map>
#include <vector>

#include "cocos2d.h"

struct ItemSprite {
    std::string name;
    int quantity;
    std::string description;
    std::string fileName;
    cocos2d::Rect rect;
    ItemSprite(std::string name, int quantity, std::string description,
               std::string fileName, cocos2d::Rect rect)
        : name(name),
          quantity(quantity),
          description(description),
          fileName(fileName),
          rect(rect) {}
};

class Bag {
public:
    Bag();
    ~Bag();

    bool isEmpty() const;
    bool isFull() const;

    void addItem(const int& key, ItemSprite* item);
    ItemSprite* findItem(const int& key);
    void removeItem(const int& key);
    void replaceItem(const int& key,
                     ItemSprite* newItem);  // 更新物品信息
    void swapItems(int key1, int key2);     // 交换两个物品的位置
    void ReduceItem(int key);
    void add(ItemSprite* newItem);

    void bagInit();
    // 新增的 getItems 方法
    const std::unordered_map<int, ItemSprite*>& getItems() const;
    ItemSprite* getItems(int index);

private:
    std::unordered_map<int, ItemSprite*> items_;  // 存储物品
    int currentNum_;
    int maxNum_;
};
#endif  // BAG_H_
