#ifndef ANIMALS_H_
#define ANIMALS_H_

#include <string>
#include <vector>
#include "cocos2d.h"

#include "game/core/i_time_observer.h" // 假设路径调整过
#include "game/farming/crops/agriculture.h"
#include "game/core/position.h"

namespace th_valley {

class Animals : public Agriculture, public ITimeObserver {
public:
    Animals();
    virtual ~Animals();

    // --- ITimeObserver 实现 ---
    void OnTimeTick(WorldTime* time) override;
    void OnDayChanged(WorldTime* time) override;
    // --------------------------

    // --- 核心逻辑数据 ---
    std::string AnimalName;
    bool isHungry;
    bool isUnhappy;
    int Happiness;
    Position position;
    
    // 生长数据
    int CurrentGrowthStage;
    int MaxGrowthStage;
    int GrowthDuration; // 总生长时长(秒或Tick)
    
    // 产出数据
    std::string ProduceItemName; // 产出物品名
    int ProduceCount;

    // --- 表现层配置 (View Config) ---
    // 这些数据在具体子类的构造函数中设置，但不立即加载图片
    std::string texturePath;    // 图片路径
    cocos2d::Size frameSize;    // 单个动画帧的大小 (e.g. 32x32)
    cocos2d::Rect initialRect;  // 初始站立的帧区域
    std::vector<float> scaleRange; // {min, max}

    // --- 表现层实例 ---
    cocos2d::Sprite* AnimalSprite = nullptr; 
    cocos2d::Vector<cocos2d::SpriteFrame*> animFrames[5]; // 存储动画帧

    // --- 核心方法 ---
    
    // [新增] 初始化视图：解耦的关键
    // 在 Manager Spawn 时调用，而不是在构造函数中调用
    virtual void InitializeView(cocos2d::Node* parent);

    // 个体行为
    virtual void Stroke();
    virtual bool Eat(const std::string& EatFood);
    virtual void AnimalAutomaticUpdate(WorldTime* time);

protected:
    // 辅助：根据 texturePath 和 frameSize 切割动画帧
    void GenerateFrames();
};

} // namespace th_valley

#endif // ANIMALS_H_