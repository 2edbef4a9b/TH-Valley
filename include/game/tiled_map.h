#ifndef TILED_MAP_H_
#define TILED_MAP_H_

#include <unordered_map>

#include "cocos2d.h"

namespace th_valley {

class TiledMap final : public cocos2d::TMXTiledMap {
public:
    TiledMap() = default;
    ~TiledMap() override = default;
    TiledMap(const TiledMap& other) = delete;
    TiledMap& operator=(const TiledMap& other) = delete;
    TiledMap(TiledMap&& other) = delete;
    TiledMap& operator=(TiledMap&& other) = delete;

    bool initWithTMXFile(const std::string& tmxFile);
    void setPlayerPos(cocos2d::Vec2 pos);

    cocos2d::Vec2 tileCoordFromPos(cocos2d::Vec2 pos);

    bool isCollision(cocos2d::Vec2 pos, std::string LayerName = "Paths");
    bool isCollisionAtAnyLayer(cocos2d::Vec2 pos);

    bool isPortal(cocos2d::Vec2 pos, std::string ObjectLayerName = "Objects");
    void triggerPortalEvent(const std::string& portalName);

    void setViewpointCenter(cocos2d::Vec2 pos);

    cocos2d::Vec2 getPos();

    void checkEventsAndTrigger(cocos2d::Vec2 tileCoord);

    void updateTileAt(cocos2d::Vec2 tileCoord, int newGID,
                      std::string LayerName);

    void onEnter() override;
    void createMiniMap();
    void save();
    void load();

    void update(float delta) override;

    static TiledMap* create(const std::string& tmxFile);

private:
    cocos2d::TMXTiledMap* tileMap_{};
    std::unordered_map<std::string, cocos2d::TMXLayer*> mapLayer_;
    cocos2d::TMXObjectGroup* objectGroup_{};
    cocos2d::Vec2 playerPos_;
    cocos2d::Sprite* playerSprite_{};
    cocos2d::ValueMap playerObject_;

    bool isKeyPressedW_{};
    bool isKeyPressedA_{};
    bool isKeyPressedS_{};
    bool isKeyPressedD_{};
};

}  // namespace th_valley

#endif  // TILED_MAP_H_
