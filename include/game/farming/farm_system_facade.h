#ifndef FARM_SYSTEM_FACADE_H_
#define FARM_SYSTEM_FACADE_H_

#include <string>
#include "cocos2d.h"

#include "game/worldtime.h"
#include "game/weather.h"
#include "game/crop_manager.h"
#include "game/animal_manager.h"
#include "game/statistics_manager.h"
#include "utility/position.h"

namespace th_valley {

class FarmSystemFacade {
public:
    FarmSystemFacade();
    ~FarmSystemFacade();

    // 禁用拷贝
    FarmSystemFacade(const FarmSystemFacade&) = delete;
    FarmSystemFacade& operator=(const FarmSystemFacade&) = delete;

    // --- 初始化 ---
    // map_root_node: 用于渲染 Sprite 的父节点 (通常是 TiledMap 或 ObjectLayer)
    void InitSystem(cocos2d::Node* map_root_node);

    // --- 游戏循环 ---
    // 在 TiledMap::update 中调用
    void UpdateGame(float dt);

    // --- 玩家交互接口 (核心) ---
    // 当玩家点击地图时调用
    // grid_pos: 点击的网格坐标
    // world_pos: 点击的世界像素坐标 (用于判定动物点击)
    // selected_tool: 当前手持工具名称 (如 "Hoe", "Seeds", "WateringCan")
    void OnInteract(const Position& grid_pos, const cocos2d::Vec2& world_pos, const std::string& selected_tool);

    // 专门的种植接口 (通常由 OnInteract 内部调用，也可供调试使用)
    bool PlantCrop(const Position& pos, const std::string& seed_name);

    // --- 数据保存/加载接口 ---
    void SaveData(const std::string& save_file);
    void LoadData(const std::string& save_file);

    // --- 子系统访问器 (供 UI 使用) ---
    WorldTime* GetWorldTime() const { return time_system_; }
    Weather* GetWeather() const { return weather_system_; }
    StatisticsManager* GetStatistics() const { return statistics_manager_; }
    // CropManager/AnimalManager 通常不需要暴露给 UI，除非为了调试显示碰撞箱

private:
    // 环境系统引用 (使用全局单例或注入)
    WorldTime* time_system_{nullptr};
    Weather* weather_system_{nullptr};

    // 子系统管理器 (由 Facade 拥有并管理生命周期)
    CropManager* crop_manager_{nullptr};
    AnimalManager* animal_manager_{nullptr};
    StatisticsManager* statistics_manager_{nullptr};

    // 内部辅助逻辑
    void HandleCropInteraction(const Position& pos, const std::string& tool);
    void HandleAnimalInteraction(const cocos2d::Vec2& world_pos, const std::string& tool);
};

} // namespace th_valley

#endif // FARM_SYSTEM_FACADE_H_