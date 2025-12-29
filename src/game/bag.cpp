#include "game/bag.h"

Bag::Bag() {
    maxNum_ =
        20;  // Assume backpack has 20 slots - Refactored with Flyweight Pattern
    currentNum_ = 0;
    money = 5000;
}

Bag::~Bag() {
    // Release all ItemStack memory - Refactored with Flyweight Pattern
    for (auto& pair : slots_) {
        delete pair.second;
    }
    slots_.clear();
}

bool Bag::isEmpty() const { return slots_.empty(); }

bool Bag::isFull() const { return slots_.size() >= maxNum_; }

// Add by existing Stack - Refactored with Flyweight Pattern
bool Bag::addItem(const int& slotIndex, ItemStack* stack) {
    if (isFull() || slotIndex < 0 || slotIndex >= maxNum_) return false;

    // If the slot already has an item, simple handling is to directly overwrite
    // or reject Here assume the logic is to overwrite or place at that position
    // - Refactored with Flyweight Pattern
    if (slots_.find(slotIndex) != slots_.end()) {
        delete slots_[slotIndex];  //- Refactored with Flyweight Pattern
    }

    slots_[slotIndex] = stack;
    return true;
}

// Add by name and quantity (lookup definition via factory)
// - Refactored with Flyweight Pattern
bool Bag::addItemByData(const int& slotIndex, std::string itemName,
                        int quantity) {
    ItemDefinition* def =
        ItemFactory::getInstance()->GetItemDefinition(itemName);
    if (!def) return false;

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
        delete it
            ->second;  // Release memory - Refactored with Flyweight Pattern
        slots_.erase(it);
    }
}

void Bag::swapItems(int slot1, int slot2) {
    // Check index validity - Refactored with Flyweight Pattern
    if (slot1 < 0 || slot1 >= maxNum_ || slot2 < 0 || slot2 >= maxNum_) return;

    auto it1 = slots_.find(slot1);
    auto it2 = slots_.find(slot2);

    ItemStack* item1 = (it1 != slots_.end()) ? it1->second : nullptr;
    ItemStack* item2 = (it2 != slots_.end()) ? it2->second : nullptr;

    // Swap logic - Refactored with Flyweight Pattern
    if (item1)
        slots_[slot2] = item1;
    else
        slots_.erase(slot2);

    if (item2)
        slots_[slot1] = item2;
    else
        slots_.erase(slot1);
}

void Bag::ReduceItem(int slotIndex, int count) {
    auto it = slots_.find(slotIndex);
    if (it != slots_.end()) {
        ItemStack* stack = it->second;
        stack->Remove(count);

        // If quantity becomes zero, remove from backpack
        // - Refactored with Flyweight Pattern
        if (stack->getQuantity() <= 0) {
            removeItem(slotIndex);
        }
    }
}

const std::map<int, ItemStack*>& Bag::getSlots() const { return slots_; }

void Bag::bagInit() {
    // Clear backpack - Refactored with Flyweight Pattern
    for (auto& pair : slots_) {
        delete pair.second;
    }
    slots_.clear();

    // Example: Give some items during initialization
    // Note: Need to register "Hoe" in factory with LoadDefinition first
    addItemByData(0, "Hoe", 1);
    addItemByData(1, "Seed", 10);
}
