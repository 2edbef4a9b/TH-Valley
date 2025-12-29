#ifndef TOOL_STRATEGIES_H_
#define TOOL_STRATEGIES_H_

#include "cocos2d.h"
#include "game/entity.h" // Requires Direction enum - Refactored with Strategy Pattern
#include <list>
#include <string>

namespace th_valley {

class Avatar; // Forward declaration to avoid circular reference - Refactored with Strategy Pattern

// --- Abstract Strategy Interface - Refactored with Strategy Pattern ---
class IToolStrategy {
public:
    virtual ~IToolStrategy() = default;
    // Defines common interface for tool usage - Refactored with Strategy Pattern
    virtual void Use(Avatar* user, cocos2d::Vec2 targetPos) = 0;

    // --- Object Pool Pattern Implementation - Refactored with Object Pool Pattern ---
    // Static pool shared by all strategies
    static void CleanupPool(); // Cleanup the object pool - Refactored with Object Pool Pattern

protected:
    static cocos2d::Sprite* GetEffectSprite(const std::string& textureName, const cocos2d::Rect& rect);
    static void ReturnEffectSprite(cocos2d::Sprite* sprite);

private:
    static std::list<cocos2d::Sprite*> effect_pool_;
};

// --- Concrete Strategy: Watering Can - Refactored with Strategy Pattern ---
class WateringCanStrategy : public IToolStrategy {
public:
    void Use(Avatar* user, cocos2d::Vec2 targetPos) override;
};

// --- Concrete Strategy: Hoe - Refactored with Strategy Pattern ---
class HoeStrategy : public IToolStrategy {
public:
    void Use(Avatar* user, cocos2d::Vec2 targetPos) override;
};

// --- Concrete Strategy: Fishing Rod - Refactored with Strategy Pattern ---
class FishingRodStrategy : public IToolStrategy {
public:
    void Use(Avatar* user, cocos2d::Vec2 targetPos) override;
};

// --- Concrete Strategy: Weapon - Refactored with Strategy Pattern ---
class WeaponStrategy : public IToolStrategy {
public:
    explicit WeaponStrategy(std::string weaponName) : weaponName_(std::move(weaponName)) {}
    void Use(Avatar* user, cocos2d::Vec2 targetPos) override;
private:
    std::string weaponName_;
};

} // namespace th_valley

#endif // TOOL_STRATEGIES_H_