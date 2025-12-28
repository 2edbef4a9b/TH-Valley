#ifndef SPECIFIC_CROPS_H_
#define SPECIFIC_CROPS_H_

#include "game/crops.h"
#include "game/item_system.h" // 引入享元系统

namespace th_valley {

// --- Strawberry ---
class Strawberry : public Crops {
public:
    Strawberry() {
        CropName = "Strawberry";
        // CropType = "Fruit"; // 可以保留，或者放入 ItemDefinition
        
        // 状态初始化
        CurrentGrowthStage = 0;
        MaxGrowthStage = 3;
        
        // 生长数据
        GrowthDuration = {50, 50, 50, 50}; // 对应 UML 或策划表
        GrowthStage = {"Seed", "Seedling", "Maturation", "Ripened"};
        
        // 视图数据 (Sprite Frame)
        // 注意：实际项目中建议将这些硬编码数据移至 JSON 配置文件，由 Factory 读取
        frameRect.resize(4);
        frameRect[0].setRect(1, 593, 13, 14);
        frameRect[1].setRect(36, 597, 8, 8);
        frameRect[2].setRect(81, 587, 16, 23);
        frameRect[3].setRect(97, 587, 16, 24);

        // 创建初始精灵 (使用第一阶段)
        // 注意：资源路径最好用常量
        CropSprite = cocos2d::Sprite::create("assets/Crops/crops.png", frameRect[0]);

        // --- 享元模式修正 ---
        // 删除: Fruit = new ItemSprite(...)
        // 改为: 记录产出物的 ID/Name，收获时去 ItemFactory 查
        // 假设基类 Crops 有一个 std::string productItemName;
        productItemName = "Strawberry"; 
        output_count = 8;

        // 属性设置
        isNormal = true;
        GrowthSpeed = 1;
        
        // 种植要求
        SeasonRequirement = {"Spring"};
        SoilRequirement = {"Arable"};
    }
};

// --- Carrot ---
class Carrot : public Crops {
public:
    Carrot() {
        CropName = "Carrot";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 3;
        GrowthDuration = {60 * 60 * 24 * 1, 60 * 60 * 24 * 1, 60 * 60 * 24 * 1, 60 * 60 * 24 * 7};
        
        frameRect.resize(4);
        frameRect[0].setRect(2, 785, 11, 13);
        frameRect[1].setRect(35, 785, 10, 20);
        frameRect[2].setRect(51, 783, 10, 22);
        frameRect[3].setRect(65, 779, 13, 25);
        
        CropSprite = cocos2d::Sprite::create("assets/Crops/crops.png", frameRect[0]);
        
        productItemName = "Carrot";
        output_count = 1;

        SeasonRequirement = {"Spring", "Summer", "Autumn"};
    }
};

// --- Potato ---
class Potato : public Crops {
public:
    Potato() {
        CropName = "Potato";
        CurrentGrowthStage = 0;
        MaxGrowthStage = 3;
        GrowthDuration = {50, 50, 50, 50};
        
        frameRect.resize(4);
        frameRect[0].setRect(130, 54, 10, 7);
        frameRect[1].setRect(180, 50, 10, 9);
        frameRect[2].setRect(208, 45, 14, 19);
        frameRect[3].setRect(225, 42, 14, 22);
        
        CropSprite = cocos2d::Sprite::create("assets/Crops/crops.png", frameRect[0]);
        
        productItemName = "Potato";
        output_count = 1;

        SeasonRequirement = {"Spring", "Summer", "Autumn"};
    }
};

} // namespace th_valley

#endif // SPECIFIC_CROPS_H_