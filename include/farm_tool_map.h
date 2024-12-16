#ifndef FARM_TOOL_MAP_H
#define FARM_TOOL_MAP_H

#include <string>
#include <unordered_map>

#include "map.h"

// 工具与 GID 的映射表
//static std::unordered_map<std::string, int> toolGidMapping = {
//    {"hoe", 3},           // 锄头对应的耕地瓦片 GID
//    {"watering_can", 4},  // 水壶对应的湿润地面瓦片 GID
//    {"axe", 5},           // 斧头对应的砍树 GID
//    {"pickaxe", 6},       // 镐子对应的敲击石头 GID
//    {"seed", 7}           // 种子对应的播种 GID
//};

class FarmToolMap : public Map {
//private:
//    struct CropData {
//        int growthStage;  // 作物生长阶段
//        int maxGrowthStage;  // 最大生长阶段
//        float timeSinceLastUpdate;  // 上次更新时间
//        float growthTime;           // 生长时间
//        CropData(int maxGrowth, float growthInterval)
//            : growthStage(0),
//              maxGrowthStage(maxGrowth),
//              timeSinceLastUpdate(0.0f),
//              growthTime(growthInterval) {}
//    };
//
//    std::unordered_map<cocos2d::Vec2, CropData>
//        crops;  // 用于存储作物的生长状态
//    
//    void growCrops(float delta);
//
//public:
//    FarmToolMap() = default;
//    ~FarmToolMap() override = default;
//
//    void addSprite(cocos2d::Sprite* sprite, cocos2d::Vec2 tileCoord,
//                   const std::string& layerName);  // 添加精灵
//
//    void useToolOnTile(cocos2d::Vec2 tileCoord,
//                       const std::string& toolName);  // 使用工具
//    void plantSeed(cocos2d::Vec2 tileCoord, int seedGID);  // 种植种子
//    void update(float delta) override;                     // 更新地图
//
//    CREATE_FUNC(FarmToolMap);
};

#endif  // FARM_TOOL_MAP_H
