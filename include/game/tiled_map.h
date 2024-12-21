#ifndef TILED_MAP_H_
#define TILED_MAP_H_

#include <optional>
#include <string_view>
#include <unordered_map>

#include "cocos2d.h"
#include "math/CCGeometry.h"

namespace th_valley {

class TiledMap final : public cocos2d::Node {
public:
    class Portal {
    public:
        Portal(std::string_view from_map, std::string_view to_map);

        std::string_view GetFromMap() const;
        std::string_view GetToMap() const;
        std::string GetFromPortalName() const;
        std::string GetToPortalName() const;

    private:
        std::string_view from_map_;
        std::string_view to_map_;
    };

    TiledMap() = default;
    ~TiledMap() override = default;
    TiledMap(const TiledMap& other) = delete;
    TiledMap& operator=(const TiledMap& other) = delete;
    TiledMap(TiledMap&& other) = delete;
    TiledMap& operator=(TiledMap&& other) = delete;

    static TiledMap* create(const std::string& tmxFile);

    bool InitWithTMXFile(const std::string& tmxFile);
    void CreateMiniMap();
    void Save();
    void Load();

private:
    void onEnter() override;
    void update(float delta) override;

    bool IsCollision(cocos2d::Vec2 pos, std::string LayerName = "Paths");
    bool IsCollisionAtAnyLayer(cocos2d::Vec2 pos);

    cocos2d::Vec2 GetPos();
    cocos2d::Vec2 TileCoordFromPos(cocos2d::Vec2 pos);

    std::optional<Portal> GetPortal(
        cocos2d::Vec2 pos, std::string_view ObjectLayerName = "Objects");
    cocos2d::Rect GetPortalRect(Portal portal,
                                std::string_view ObjectLayerName = "Objects");

    void SetPlayerPos(cocos2d::Vec2 pos);
    void SetViewpointCenter(cocos2d::Vec2 pos);
    void UpdateTileAt(cocos2d::Vec2 tileCoord, int newGID,
                      std::string LayerName);

    cocos2d::TMXTiledMap* tileMap_{};
    std::unordered_map<std::string, cocos2d::TMXLayer*> mapLayer_;
    cocos2d::TMXObjectGroup* objectGroup_{};
    cocos2d::Vec2 playerPos_;
    cocos2d::Sprite* playerSprite_{};

    bool isKeyPressedW_{};
    bool isKeyPressedA_{};
    bool isKeyPressedS_{};
    bool isKeyPressedD_{};
};

}  // namespace th_valley

#endif  // TILED_MAP_H_
