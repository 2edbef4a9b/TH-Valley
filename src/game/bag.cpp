#include "game/bag.h"

Bag::Bag() : currentNum_(0), maxNum_(40) { bagInit(); }

Bag::~Bag() {}

bool Bag::isEmpty() const { return currentNum_ == 0; }

bool Bag::isFull() const { return currentNum_ >= maxNum_; }

void Bag::addItem(const int& key, ItemSprite* item) {
    if (isFull()) {
        // 背包已满，无法添加物品
        return;
    }
    auto it = items_.find(key);
    if (it != items_.end()) {
        // 物品已存在，增加数量
        it->second->quantity += item->quantity;
        delete item;  // 释放新传入的物品
    } else {
        // 添加新物品
        items_[key] = item;
        ++currentNum_;
    }
}

ItemSprite* Bag::findItem(const int& key) {
    auto it = items_.find(key);
    if (it != items_.end()) {
        return it->second;
    }
    return nullptr;
}

void Bag::removeItem(const int& key) {
    auto it = items_.find(key);
    if (it != items_.end()) {
        delete it->second;
        items_.erase(it);
        --currentNum_;
    }
}

void Bag::ReduceItem(const int key) {
    auto it = items_.find(key);
    if (it != items_.end()) {
        it->second->quantity--;
        if (it->second->quantity == 0) removeItem(key);
    }
}

void Bag::replaceItem(const int& key, ItemSprite* newItem) {
    if (newItem == nullptr) return;
    auto it = items_.find(key);
    if (it != items_.end()) {
        delete it->second;
        it->second = newItem;
    }
}

void Bag::swapItems(int key1, int key2) {
    auto it1 = items_.find(key1);
    auto it2 = items_.find(key2);
    if (it1 != items_.end() && it2 != items_.end()) {
        std::swap(it1->second, it2->second);
    } else if (it1 != items_.end()) {
        items_[key2] = it1->second;
        items_.erase(it1);
    } else if (it2 != items_.end()) {
        items_[key1] = it2->second;
        items_.erase(it2);
    }
}

void Bag::bagInit() {
    ItemSprite* Axe = new ItemSprite(
        "Axe", 1, "Used for chopping wood", "assets/TileSheets/tools.png",
        cocos2d::Rect(0 + 5 * 16, 32 + 4 * 32, 16, 16));
    items_[0] = Axe;
    ItemSprite* Hoe = new ItemSprite(
        "Hoe", 3, "Used for chopping wood", "assets/TileSheets/tools.png",
        cocos2d::Rect(0 + 5 * 16, 32 + 0 * 32, 16, 16));
    items_[1] = Hoe;

    ItemSprite* StrawberrySeed =
        new ItemSprite("StrawberrySeed", 12, "Used for planting strawberries",
                       "assets/Crops/crops.png", cocos2d::Rect(1, 593, 13, 14));
    items_[2] = StrawberrySeed;
    ItemSprite* CarrotSeed =
        new ItemSprite("CarrotSeed", 68, "Used for planting carrots",
                       "assets/Crops/crops.png", cocos2d::Rect(2, 785, 11, 13));
    items_[15] = CarrotSeed;
    ItemSprite* PotatoSeed =
        new ItemSprite("PotatoSeed", 64, "Used for planting potatoes",
                       "assets/Crops/crops.png", cocos2d::Rect(130, 54, 10, 7));
    items_[38] = PotatoSeed;

    for (const auto& [i, item] : items_) {
        CCLOG("Item %s added to bag index %d", item->name.c_str(), i);
    }
}

const std::unordered_map<int, ItemSprite*>& Bag::getItems() const {
    return items_;
}

ItemSprite* Bag::getItems(int index) {
    if (items_[index])
        return items_[index];
    else
        return nullptr;
}
