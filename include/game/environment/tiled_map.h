#ifndef TILED_MAP_H_
#define TILED_MAP_H_

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <map>

#include "cocos2d.h"

// 引入依赖
#include "game/avatar.h"
#include "game/entity.h"
#include "game/citizen.h"
#include "utility/position.h"

// 前向声明
namespace th_valley {
    class FarmSystemFacade; 
    class MapController;
}

namespace th_valley {

class TiledMap final : public cocos2d::Node {
public:
    // 传送门定义
    class Portal {
    public:
        explicit Portal(const std::string& portal_name);
        [[nodiscard]] std::string GetFromMap() const;
        [[nodiscard]] std::string GetToMap() const;
        [[nodiscard]] std::string GetPortalName() const;
        [[nodiscard]] std::string GetOppositePortalName() const;
    private:
        std::string from_map_;
        std::string to_map_;
    };

    TiledMap();
    ~TiledMap() override;
    
    // 禁用拷贝
    TiledMap(const TiledMap&) = delete;
    TiledMap& operator=(const TiledMap&) = delete;

    static TiledMap* create(const std::string& tmxFile);
    bool InitWithTMXFile(const std::string& tmxFile);
    
    void onEnter() override;
    void update(float delta) override;

    // --- 输入处理 ---
    // 核心：处理点击，将世界坐标转换为网格坐标，转发给 FarmSystemFacade
    void OnClick(const cocos2d::Vec2& worldPos, const std::string& selectedTool);
    
    void HandleKeyboardInput(); 

    // --- 地图功能 ---
    cocos2d::Rect GetPortalRect(Portal portal, std::string_view ObjectLayerName = "Objects");
    void SetPlayerPos(cocos2d::Vec2 pos);
    void SetTeleportStatus(bool status);
    
    cocos2d::TMXTiledMap* getTiledMap() { return tiled_map_; }
    std::string GetMapName() const { return map_name_; }
    void SetMapName(const std::string& name) { map_name_ = name; }
    
    // 获取 FarmSystemFacade 指针 (供 MapController 保存数据使用)
    FarmSystemFacade* GetFarmSystem() const { return farm_system_; }

    // --- 辅助计算 ---
    cocos2d::Vec2 GetPlayerPos();
    cocos2d::Vec2 TileCoordFromPos(cocos2d::Vec2 pos);
    cocos2d::Vec2 PosFromtileCoord(Position pos);
    bool IsCollision(cocos2d::Vec2 pos, std::string LayerName = "Paths");
    bool IsCollisionAtAnyLayer(cocos2d::Vec2 pos);

    // --- NPC 相关 ---
    void SpawnCitizen(Citizen* citizen);
    void initCitizenPosition();
    void updateCitizenSprites(float dt);

    // --- 数据持久化 ---
    void Save(const std::string& file_name);
    void Load(const std::string& file_name);

private:
    cocos2d::TMXTiledMap* tiled_map_{nullptr};
    std::unordered_map<std::string, cocos2d::TMXLayer*> map_layer_;
    cocos2d::TMXObjectGroup* object_group_{nullptr};
    
    std::string map_name_;
    
    cocos2d::Vec2 player_pos_;
    Avatar avatar{};
    bool is_teleporting_{false};

    bool is_key_pressed_w_{false};
    bool is_key_pressed_a_{false};
    bool is_key_pressed_s_{false};
    bool is_key_pressed_d_{false};

    std::vector<Citizen*> MapCitizens;
    std::vector<cocos2d::Sprite*> CitizensSprite;
    std::map<cocos2d::Sprite*, Citizen*> SpritetoCitizen;

    // --- 系统外观 ---
    // TiledMap 拥有并管理 FarmSystemFacade 的生命周期
    FarmSystemFacade* farm_system_{nullptr};

    // 私有辅助函数
    void CreateMiniMap();
    std::optional<Portal> GetPortal(cocos2d::Vec2 pos, std::string_view ObjectLayerName = "Objects") const;
    void SetViewpointCenter(cocos2d::Vec2 pos);
    void UpdateTileAt(cocos2d::Vec2 tileCoord, int newGID, std::string LayerName);
    void ShowCitizenInfomation(cocos2d::Sprite* Citizen, const cocos2d::Vec2& InfoPosition, int& priority);
    
    int priority = 255; 
};

}  // namespace th_valley

#endif  // TILED_MAP_H_