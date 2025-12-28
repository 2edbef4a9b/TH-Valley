#ifndef ITEM_SYSTEM_H_
#define ITEM_SYSTEM_H_

#include <string>
#include <map>
#include "cocos2d.h"

// 1. ItemDefinition (享元对象：只读数据)
class ItemDefinition {
public:
    std::string name;
    std::string description;
    std::string texturePath;
    cocos2d::Rect rect;

    ItemDefinition(std::string name, std::string desc, std::string path, cocos2d::Rect r)
        : name(name), description(desc), texturePath(path), rect(r) {}
};

// 2. ItemFactory (工厂：管理享元对象)
class ItemFactory {
public:
    // 获取单例实例
    static ItemFactory* getInstance();
    
    // 获取道具定义 (如果不存在返回 nullptr)
    ItemDefinition* GetItemDefinition(const std::string& name);

    // 注册道具定义 (通常在游戏启动时从 JSON/XML 加载)
    void LoadDefinition(const std::string& name, const std::string& desc, 
                        const std::string& path, cocos2d::Rect rect);

private:
    ItemFactory() = default;
    std::map<std::string, ItemDefinition*> definitions_;
};

// 3. ItemStack (堆叠：引用定义 + 数量)
class ItemStack {
public:
    ItemStack(ItemDefinition* def, int qty);
    
    // 获取定义数据的快捷方式
    const std::string& getName() const;
    const std::string& getDescription() const;
    const std::string& getTexturePath() const;
    const cocos2d::Rect& getRect() const;
    
    // 数量操作
    int getQuantity() const;
    void Add(int count);
    void Remove(int count); // 减少数量，返回剩余是否 > 0

    ItemDefinition* getDefinition() const { return definition_; }

private:
    ItemDefinition* definition_; // 指向享元的指针
    int quantity_;
};

#endif // ITEM_SYSTEM_H_