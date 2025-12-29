#ifndef ITEM_SYSTEM_H_
#define ITEM_SYSTEM_H_

#include <string>
#include <map>
#include "cocos2d.h"

// 1. ItemDefinition (Flyweight object: read-only data) - Refactored with Flyweight Pattern
class ItemDefinition {
public:
    std::string name;
    std::string description;
    std::string texturePath;
    cocos2d::Rect rect;

    ItemDefinition(std::string name, std::string desc, std::string path, cocos2d::Rect r)
        : name(name), description(desc), texturePath(path), rect(r) {}
};

// 2. ItemFactory (Factory: manages flyweight objects) - Refactored with Flyweight Pattern
class ItemFactory {
public:
    // Get singleton instance - Refactored with Flyweight Pattern
    static ItemFactory* getInstance();
    
    // Get item definition (returns nullptr if doesn't exist) - Refactored with Flyweight Pattern
    ItemDefinition* GetItemDefinition(const std::string& name);

    // Register item definition (usually loaded from JSON/XML at game startup) - Refactored with Flyweight Pattern
    void LoadDefinition(const std::string& name, const std::string& desc,
                        const std::string& path, cocos2d::Rect rect);

private:
    ItemFactory() = default;
    std::map<std::string, ItemDefinition*> definitions_;
};

// 3. ItemStack (Stack: references definition + quantity) - Refactored with Flyweight Pattern
class ItemStack {
public:
    ItemStack(ItemDefinition* def, int qty);
    
    // Get definition data shortcuts - Refactored with Flyweight Pattern
    const std::string& getName() const;
    const std::string& getDescription() const;
    const std::string& getTexturePath() const;
    const cocos2d::Rect& getRect() const;
    
    // Quantity operations - Refactored with Flyweight Pattern
    int getQuantity() const;
    void Add(int count);
    void Remove(int count); // Reduce quantity, return whether remaining > 0 - Refactored with Flyweight Pattern

    ItemDefinition* getDefinition() const { return definition_; }

private:
    ItemDefinition* definition_; // Pointer to flyweight - Refactored with Flyweight Pattern
    int quantity_;
};

#endif // ITEM_SYSTEM_H_