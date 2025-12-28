#include "game/item_system.h"

// --- ItemFactory ---

ItemFactory* ItemFactory::getInstance() {
    static ItemFactory instance;
    return &instance;
}

ItemDefinition* ItemFactory::GetItemDefinition(const std::string& name) {
    auto it = definitions_.find(name);
    if (it != definitions_.end()) {
        return it->second;
    }
    CCLOG("Error: Item Definition not found for %s", name.c_str());
    return nullptr;
}

void ItemFactory::LoadDefinition(const std::string& name, const std::string& desc,
                                 const std::string& path, cocos2d::Rect rect) {
    if (definitions_.find(name) == definitions_.end()) {
        definitions_[name] = new ItemDefinition(name, desc, path, rect);
    }
}

// --- ItemStack ---

ItemStack::ItemStack(ItemDefinition* def, int qty) 
    : definition_(def), quantity_(qty) {}

const std::string& ItemStack::getName() const { return definition_->name; }
const std::string& ItemStack::getDescription() const { return definition_->description; }
const std::string& ItemStack::getTexturePath() const { return definition_->texturePath; }
const cocos2d::Rect& ItemStack::getRect() const { return definition_->rect; }

int ItemStack::getQuantity() const { return quantity_; }

void ItemStack::Add(int count) {
    quantity_ += count;
}

void ItemStack::Remove(int count) {
    quantity_ -= count;
    if (quantity_ < 0) quantity_ = 0;
}