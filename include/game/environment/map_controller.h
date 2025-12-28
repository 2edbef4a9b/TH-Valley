#ifndef MAP_CONTROLLER_H_
#define MAP_CONTROLLER_H_

#include <string>
#include <string_view>
#include <vector>

#include "cocos2d.h"
#include "math/Vec2.h"
#include "utility/position.h"

// 前向声明 TiledMap，避免包含 tiled_map.h 导致循环依赖
namespace th_valley {
    class TiledMap; 
}

namespace th_valley {

class MapController {
public:
    // 禁用拷贝和赋值
    MapController(const MapController&) = delete;
    MapController& operator=(const MapController&) = delete;
    MapController(MapController&&) = delete;
    MapController& operator=(MapController&&) = delete;

    // 获取单例
    static MapController& GetInstance();

    // --- 核心逻辑 ---

    // 加载并切换地图
    void LoadTiledMap(const std::string& tiled_map_name, cocos2d::Node* parent);

    // 保存当前地图状态 (委托给 TiledMap -> Facade)
    void SaveCurrentMap();

    // 触发传送 (通常由 TiledMap 检测到碰撞后调用)
    void TriggerTeleport(const std::string& portal_name);

    // --- 玩家数据管理 ---
    void SetPlayerPos(cocos2d::Vec2 pos);
    cocos2d::Vec2 GetPlayerPos() const;

    void LoadPlayerInfo();
    void SavePlayerInfo();

    std::string GetPlayerName() const { return player_name_; }
    std::string GetPlayerMap() const { return player_map_; }

    // --- 辅助功能 ---
    void LoadRainEffect(const std::string& tiled_map_name);

    // 获取当前持有的 TiledMap 指针
    TiledMap* GetTiledMap() const { return game_tiled_map_; }

private:
    MapController() = default;
    ~MapController() = default;

    const std::string kTiledMapPathPrefix = "assets/maps/";
    const std::string kTiledMapPathSuffix = ".tmx";

    TiledMap* game_tiled_map_{nullptr};

    std::string player_name_;
    std::string player_map_; 
    cocos2d::Vec2 player_pos_;
};

}  // namespace th_valley

#endif  // MAP_CONTROLLER_H_