#ifndef CROPS_H_
#define CROPS_H_

#include <string>
#include <vector>
#include "cocos2d.h"

#include "i_time_observer.h"
#include "game/agriculture.h" // 包含基类
#include "utility/position.h"

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

    // 个体属性
    std::string CropName;
    int CurrentGrowthStage;
    int MaxGrowthStage;
    std::vector<int> GrowthDuration; // 每个阶段所需的 tick 数
    
    // 渲染相关
    cocos2d::Sprite *CropSprite; 
    Position position;

    // 状态标志
    bool isWatered;
    bool isDead;
    bool isFrozen;
    bool isFertilize;

    // 个体行为接口
    void setCurrentGrowthStage(int stage);
    
    // 生长逻辑 (由 OnTimeTick 调用)
    virtual void CropAutomaticUpdate(WorldTime* time); 
    
    // 交互行为
    virtual void CropWatering(); 
    virtual void Harvest();
    
    // Getter
    const std::string& getCropName() const { return CropName; }
};

} // namespace th_valley

#endif // CROPS_H_