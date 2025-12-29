#ifndef BAG_H_
#define BAG_H_

#include <string>
#include <map> 
#include "cocos2d.h"
#include "game/item_system.h" // Include the system defined above - Refactored with Flyweight Pattern

class Bag {
public:
    Bag();
    ~Bag();

    bool isEmpty() const;
    bool isFull() const;

    // Add item: can pass Stack pointer directly, or create by name - Refactored with Flyweight Pattern
    bool addItem(const int& slotIndex, ItemStack* stack);
    bool addItemByData(const int& slotIndex, std::string itemName, int quantity);

    ItemStack* findItem(const int& slotIndex);
    void removeItem(const int& slotIndex);
    
    // Swap positions - Refactored with Flyweight Pattern
    void swapItems(int slot1, int slot2);
    
    // Reduce items (e.g. used one) - Refactored with Flyweight Pattern
    void ReduceItem(int slotIndex, int count = 1);

    // Get all items (note type change: ItemSprite -> ItemStack) - Refactored with Flyweight Pattern
    const std::map<int, ItemStack*>& getSlots() const;
    
    // Money system remains unchanged - Refactored with Flyweight Pattern
    void SpendMoney(const int& value) { money -= value; };
    void EarnMoney(const int& value) { money += value; }
    int getMoney() const { return money; }

    void bagInit(); // Initialize or reset backpack - Refactored with Flyweight Pattern

private:
    // Core storage structure changed: - Refactored with Flyweight Pattern
    // Key: backpack slot index (int) - Refactored with Flyweight Pattern
    // Value: item stack (ItemStack*) - Refactored with Flyweight Pattern
    std::map<int, ItemStack*> slots_; 
    
    int maxNum_; // Maximum slot count - Refactored with Flyweight Pattern
    int currentNum_;
    int money;
};

#endif // BAG_H_