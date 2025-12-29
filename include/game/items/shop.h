#ifndef SHOP_H_
#define SHOP_H_

#include <map>
#include <string>
#include <vector>
#include "game/items/bag.h"
#include "game/items/item_system.h"
#include "game/farming/animals/animal_factory.h"

// Shop logic class - Refactored with Flyweight Pattern
class Shop {
public:
    Shop() = default;
    virtual ~Shop() = default;

    // Get product list <index, item stack> - Refactored with Flyweight Pattern
    const std::map<int, ItemStack*>& GetStock() const { return stock_; }
    
    // Get product price - Refactored with Flyweight Pattern
    int GetPrice(int index) const {
        auto it = price_list_.find(index);
        return (it != price_list_.end()) ? it->second : 0;
    }

    // Purchase logic - Refactored with Flyweight Pattern
    // targetBag: player's backpack - Refactored with Flyweight Pattern
    // index: product index - Refactored with Flyweight Pattern
    bool Purchase(int index, Bag* targetBag) {
        if (stock_.find(index) == stock_.end()) return false;
        
        int price = GetPrice(index);
        if (targetBag->getMoney() < price) return false;

        ItemStack* itemToBuy = stock_[index];
        
        // Try to add to backpack (deep copy one) - Refactored with Flyweight Pattern
        ItemStack* newItem = new ItemStack(itemToBuy->getDefinition(), 1); 
        // Simple automatic empty space search logic, actual project may need to specify position - Refactored with Flyweight Pattern
        // Here we assume the add method can handle automatic stacking or positioning, or GUI layer handles it - Refactored with Flyweight Pattern
        // For compatibility with old logic, we assume Bag has an AutoAdd method, or GUI is responsible for positioning - Refactored with Flyweight Pattern
        
        // Simplified: only handle money deduction here, specific item addition is called by UI layer Bag::addItem - Refactored with Flyweight Pattern
        targetBag->SpendMoney(price);
        return true;
    }

protected:
    std::map<int, ItemStack*> stock_;
    std::map<int, int> price_list_;
};

// Specific farm shop - Refactored with Flyweight Pattern
class FarmShop : public Shop {
public:
    FarmShop() {
        // Use new ItemSystem - Refactored with Flyweight Pattern
        ItemDefinition* pigDef = ItemFactory::getInstance()->GetItemDefinition("Pig");
        if (!pigDef) {
            // If not defined, temporarily register one (should be unified at game startup) - Refactored with Flyweight Pattern
            ItemFactory::getInstance()->LoadDefinition("Pig", "Pig cub", "assets/Animals/Pig.png", cocos2d::Rect(0,0,32,32));
            pigDef = ItemFactory::getInstance()->GetItemDefinition("Pig");
        }

        // Add products - Refactored with Flyweight Pattern
        stock_[0] = new ItemStack(pigDef, 1);
        price_list_[0] = 500;
    }
};

#endif // SHOP_H_