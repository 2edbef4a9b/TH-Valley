#ifndef BAG_H_
#define BAG_H_

#include <string>
#include <map> 
#include "cocos2d.h"
#include "game/item_system.h" // 包含上面定义的系统

class Bag {
public:
    Bag();
    ~Bag();

    bool isEmpty() const;
    bool isFull() const;

    // 添加物品：可以直接传 Stack 指针，或者通过名称创建
    bool addItem(const int& slotIndex, ItemStack* stack);
    bool addItemByData(const int& slotIndex, std::string itemName, int quantity);

    ItemStack* findItem(const int& slotIndex);
    void removeItem(const int& slotIndex);
    
    // 交换位置
    void swapItems(int slot1, int slot2);
    
    // 减少物品 (例如使用了一个)
    void ReduceItem(int slotIndex, int count = 1);

    // 获取所有物品 (注意类型变化：ItemSprite -> ItemStack)
    const std::map<int, ItemStack*>& getSlots() const;
    
    // 金钱系统保持不变
    void SpendMoney(const int& value) { money -= value; };
    void EarnMoney(const int& value) { money += value; }
    int getMoney() const { return money; }

    void bagInit(); // 初始化或重置背包

private:
    // 核心存储结构改变：
    // Key: 背包格子索引 (int)
    // Value: 物品堆叠 (ItemStack*)
    std::map<int, ItemStack*> slots_; 
    
    int maxNum_; // 最大格子数
    int currentNum_;
    int money;
};

#endif // BAG_H_