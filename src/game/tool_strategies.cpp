#include "game/tool_strategies.h"
#include "game/avatar.h" // 这里包含完整的 Avatar 定义以访问其方法

namespace th_valley {

void WateringCanStrategy::Use(Avatar* user, cocos2d::Vec2 targetPos) {
    cocos2d::log("Strategy: Using Watering Can at (%f, %f)", targetPos.x, targetPos.y);
    
    // 示例逻辑：
    // 1. 计算朝向
    user->CalculateDirection(targetPos); 
    // 2. 播放浇水动画
    user->PlayAnimation("watering");
    // 3. 执行浇水后的逻辑（比如减少水量，生成水渍Entity）
}

void HoeStrategy::Use(Avatar* user, cocos2d::Vec2 targetPos) {
    cocos2d::log("Strategy: Using Hoe at (%f, %f)", targetPos.x, targetPos.y);
    // 示例逻辑：播放锄地动画，修改地图图块属性
}

void FishingRodStrategy::Use(Avatar* user, cocos2d::Vec2 targetPos) {
    cocos2d::log("Strategy: Casting Fishing Rod to (%f, %f)", targetPos.x, targetPos.y);
    // 示例逻辑：播放抛竿动画，生成浮漂
}

void WeaponStrategy::Use(Avatar* user, cocos2d::Vec2 targetPos) {
    cocos2d::log("Strategy: Attacking with %s", weaponName_.c_str());
    // 示例逻辑：播放攻击动画，进行碰撞检测
}

} // namespace th_valley