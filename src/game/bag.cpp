#include "game/bag.h"

Bag::Bag() {
    maxNum_ = 20; // 假设背包有20个格子
    currentNum_ = 0;
    money = 5000;
}

Bag::~Bag() {
    // 释放所有 ItemStack 内存
    for (auto& pair : slots_) {
        delete pair.second;
    }
    slots_.clear();
}

bool Bag::isEmpty() const {
    return slots_.empty();
}

bool Bag::isFull() const {
    return slots_.size() >= maxNum_;
}

// 通过现有的 Stack 添加
bool Bag::addItem(const int& slotIndex, ItemStack* stack) {
    if (isFull() || slotIndex < 0 || slotIndex >= maxNum_) return false;

    // 如果该格子已有物品，简单的处理是直接覆盖或拒绝
    // 这里假设逻辑是覆盖或者在该位置放置
    if (slots_.find(slotIndex) != slots_.end()) {
        delete slots_[slotIndex]; // 删除旧的
    }

    slots_[slotIndex] = stack;
    return true;
}

// 通过名称和数量添加（通过工厂查找定义）
bool Bag::addItemByData(const int& slotIndex, std::string itemName, int quantity) {
    ItemDefinition* def = ItemFactory::getInstance()->GetItemDefinition(itemName);
    if (!def) return false; // 物品定义不存在

    ItemStack* newStack = new ItemStack(def, quantity);
    return addItem(slotIndex, newStack);
}

ItemStack* Bag::findItem(const int& slotIndex) {
    auto it = slots_.find(slotIndex);
    if (it != slots_.end()) {
        return it->second;
    }
    return nullptr;
}

void Bag::removeItem(const int& slotIndex) {
    auto it = slots_.find(slotIndex);
    if (it != slots_.end()) {
        delete it->second; // 释放内存
        slots_.erase(it);
    }
}

void Bag::swapItems(int slot1, int slot2) {
    // 检查索引有效性
    if (slot1 < 0 || slot1 >= maxNum_ || slot2 < 0 || slot2 >= maxNum_) return;

    auto it1 = slots_.find(slot1);
    auto it2 = slots_.find(slot2);

    ItemStack* item1 = (it1 != slots_.end()) ? it1->second : nullptr;
    ItemStack* item2 = (it2 != slots_.end()) ? it2->second : nullptr;

    // 交换逻辑
    if (item1) slots_[slot2] = item1;
    else slots_.erase(slot2);

    if (item2) slots_[slot1] = item2;
    else slots_.erase(slot1);
}

void Bag::ReduceItem(int slotIndex, int count) {
    auto it = slots_.find(slotIndex);
    if (it != slots_.end()) {
        ItemStack* stack = it->second;
        stack->Remove(count);
        
        // 如果数量归零，从背包移除
        if (stack->getQuantity() <= 0) {
            removeItem(slotIndex);
        }
    }
}

const std::map<int, ItemStack*>& Bag::getSlots() const {
    return slots_;
}

void Bag::bagInit() {
    // 清空背包
    for (auto& pair : slots_) {
        delete pair.second;
    }
    slots_.clear();
    
    // 示例：初始化时送几个物品
    // 注意：需要先在工厂里 LoadDefinition 注册过 "Hoe"
    addItemByData(0, "Hoe", 1);
    addItemByData(1, "Seed", 10);
}