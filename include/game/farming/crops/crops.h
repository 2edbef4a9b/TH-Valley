#ifndef CROPS_H_
#define CROPS_H_

#include <string>
#include <vector>
#include "cocos2d.h"

#include "game/core/i_time_observer.h"
#include "game/farming/crops/agriculture.h" 
#include "game/core/position.h"

// 前向声明
class WorldTime;

namespace th_valley {

class Crops : public Agriculture, public ITimeObserver {
public:
    Crops(); 
    virtual ~Crops();

    // --- ITimeObserver 实现 ---
    void OnTimeTick(WorldTime* time) override;
    void OnDayChanged(WorldTime* time) override;
    // --------------------------

    // --- 核心逻辑属性 ---
    std::string CropName;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector<int> GrowthDuration; // 每个阶段所需的 tick 数
    
    // 产出属性
    std::string productItemName;
    int output_count;
    
    // 种植要求
    std::vector<std::string> SeasonRequirement;
    std::vector<std::string> SoilRequirement;

    // --- 状态标志 ---
    bool isWatered;
    bool isDead;
    bool isFrozen;
    bool isFertilize;
    bool isNormal; // 用于特殊状态判断
    int GrowthSpeed;

    // --- 表现层配置 (View Config) ---
    // 在构造函数中只设置这些数据，不创建 Sprite
    std::string texturePath; 
    std::vector<cocos2d::Rect> frameRects; // 存储每个生长阶段的切片区域
    
    // --- 表现层实例 ---
    cocos2d::Sprite *CropSprite; 
    Position position;

    // --- 核心方法 ---
    
    // [新增] 初始化视图：解耦的关键
    // 在 Manager PlantCrop 时调用
    virtual void InitializeView(cocos2d::Node* parent);

    void setCurrentGrowthStage(int stage);
    
    // 逻辑更新
    virtual void CropAutomaticUpdate(WorldTime* time); 
    
    // 交互行为
    virtual void CropWatering(); 
    virtual void Harvest();
    
    const std::string& getCropName() const { return CropName; }
};

} // namespace th_valley

#endif // CROPS_H_