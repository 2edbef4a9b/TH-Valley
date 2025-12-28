#ifndef SHOP_H_
#define SHOP_H_

#include <map>
#include <string>
#include <vector>
#include "game/items/bag.h"
#include "game/items/item_system.h"
#include "game/farming/animals/animal_factory.h"

// 商店逻辑类
class Shop {
public:
    Shop() = default;
    virtual ~Shop() = default;

    // 获取商品列表 <索引, 商品堆叠>
    const std::map<int, ItemStack*>& GetStock() const { return stock_; }
    
    // 获取商品价格
    int GetPrice(int index) const {
        auto it = price_list_.find(index);
        return (it != price_list_.end()) ? it->second : 0;
    }

    // 购买逻辑
    // targetBag: 玩家背包
    // index: 商品索引
    bool Purchase(int index, Bag* targetBag) {
        if (stock_.find(index) == stock_.end()) return false;
        
        int price = GetPrice(index);
        if (targetBag->getMoney() < price) return false;

        ItemStack* itemToBuy = stock_[index];
        
        // 尝试添加到背包 (深拷贝一份)
        ItemStack* newItem = new ItemStack(itemToBuy->getDefinition(), 1); 
        // 简单的自动寻找空位逻辑，实际项目可能需要指定位置
        // 这里假设 add 方法能处理自动堆叠或找位，或者 GUI 层处理
        // 为了兼容旧逻辑，我们假设 Bag 有一个 AutoAdd 方法，或者 GUI 负责找位置
        
        // 简化：这里只做扣钱逻辑，具体添加物品由 UI 层调用 Bag::addItem
        targetBag->SpendMoney(price);
        return true;
    }

protected:
    std::map<int, ItemStack*> stock_;
    std::map<int, int> price_list_;
};

// 具体的牧场商店
class FarmShop : public Shop {
public:
    FarmShop() {
        // 使用新的 ItemSystem
        ItemDefinition* pigDef = ItemFactory::getInstance()->GetItemDefinition("Pig");
        if (!pigDef) {
            // 如果没定义，临时注册一个（实际应在游戏启动时统一注册）
            ItemFactory::getInstance()->LoadDefinition("Pig", "Pig cub", "assets/Animals/Pig.png", cocos2d::Rect(0,0,32,32));
            pigDef = ItemFactory::getInstance()->GetItemDefinition("Pig");
        }

        // 添加商品
        stock_[0] = new ItemStack(pigDef, 1);
        price_list_[0] = 500;
    }
};

#endif // SHOP_H_