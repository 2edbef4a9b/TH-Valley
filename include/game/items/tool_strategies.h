#ifndef TOOL_STRATEGIES_H_
#define TOOL_STRATEGIES_H_

#include "cocos2d.h"
#include "game/entity.h" // 需要用到 Direction 枚举
#include <list>
#include <string>

namespace th_valley {

class Avatar; // 前向声明，避免循环引用

// --- 抽象策略接口 ---
class IToolStrategy {
public:
    virtual ~IToolStrategy() = default;
    // 定义使用工具的通用接口
    virtual void Use(Avatar* user, cocos2d::Vec2 targetPos) = 0;

    // --- Object Pool Pattern ---
    // 静态池，供所有策略共享
    static void CleanupPool(); // 清理池

protected:
    static cocos2d::Sprite* GetEffectSprite(const std::string& textureName, const cocos2d::Rect& rect);
    static void ReturnEffectSprite(cocos2d::Sprite* sprite);

private:
    static std::list<cocos2d::Sprite*> effect_pool_;
};

// --- 具体策略：喷壶 ---
class WateringCanStrategy : public IToolStrategy {
public:
    void Use(Avatar* user, cocos2d::Vec2 targetPos) override;
};

// --- 具体策略：锄头 ---
class HoeStrategy : public IToolStrategy {
public:
    void Use(Avatar* user, cocos2d::Vec2 targetPos) override;
};

// --- 具体策略：鱼竿 ---
class FishingRodStrategy : public IToolStrategy {
public:
    void Use(Avatar* user, cocos2d::Vec2 targetPos) override;
};

// --- 具体策略：普通武器/其他 ---
class WeaponStrategy : public IToolStrategy {
public:
    explicit WeaponStrategy(std::string weaponName) : weaponName_(std::move(weaponName)) {}
    void Use(Avatar* user, cocos2d::Vec2 targetPos) override;
private:
    std::string weaponName_;
};

} // namespace th_valley

#endif // TOOL_STRATEGIES_H_