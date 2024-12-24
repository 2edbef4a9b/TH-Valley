#ifndef TILED_MAP_H_
#define TILED_MAP_H_

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

#include "cocos2d.h"
#include "frontend/bag_gui.h"
#include "frontend/tool_bar.h"
#include "game/animals.h"
#include "game/avatar.h"
#include "game/citizen.h"
#include "game/entity.h"
#include "game/map_controller.h"
#include "math/CCGeometry.h"
#include "utility/position.h"

class Crops;

namespace th_valley {

class MapController;

class TiledMap final : public cocos2d::Node {
public:
    class Portal {
    public:
        explicit Portal(const std::string& portal_name);
        [[nodiscard]] std::string GetFromMap() const;
        [[nodiscard]] std::string GetToMap() const;
        [[nodiscard]] std::string GetPortalName() const;
        [[nodiscard]] std::string GetOppositePortalName() const;
        [[nodiscard]] Portal GetOppositePorta() const;

    private:
        std::string from_map_;
        std::string to_map_;
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
    void SpawnAnimal(int count);
    void SpawnCitizen(Citizen* citizen);
    void initCitizenPosition();

    cocos2d::Rect GetPortalRect(Portal portal,
                                std::string_view ObjectLayerName = "Objects");
    void SetPlayerPos(cocos2d::Vec2 pos);
    void SetTeleportStatus(bool status);
    cocos2d::TMXTiledMap* getTiledMap() { return tiled_map_; }

    // Save and Load
    void Save(const std::string& file_name);
    void Load(const std::string& file_name);
    void SavePlayerInfo();
    void LoadPlayerInfo();
    void SetMapName(const std::string& map_name);

private:
    void onEnter() override;
    void update(float delta) override;

    bool IsCollision(cocos2d::Vec2 pos, std::string LayerName = "Paths");
    bool IsCollisionAtAnyLayer(cocos2d::Vec2 pos);

    cocos2d::Vec2 GetPos();
    cocos2d::Vec2 TileCoordFromPos(cocos2d::Vec2 pos);
    cocos2d::Vec2 PosFromtileCoord(Position pos);

    int GetTileID(cocos2d::Vec2 tileCoord, std::string LayerName);
    bool PropertyCheck(int gid, std::string property);

    std::optional<Portal> GetPortal(
        cocos2d::Vec2 pos, std::string_view ObjectLayerName = "Objects") const;

    void SetViewpointCenter(cocos2d::Vec2 pos);
    void UpdateTileAt(cocos2d::Vec2 tileCoord, int newGID,
                      std::string LayerName);

    // Crop option
    void CropPlant(const Position& PlantPosition, Crops* Crop);
    void CropRemove(const Position& RemovePosition);
    void CropUpdate(const Position& UpdatePosition);
    void ShowCropInformation(Crops* Crop, const Position& InfoPosition,
                             int& priority);

    // Animal option
    void initAnimalPosition();
    void updateAnimalSprites(float dt);
    void ShowAnimalInfomation(cocos2d::Sprite* Animal,
                              const cocos2d::Vec2& InfoPosition, int& priority);

    void updateCitizenSprites(float dt);
    void ShowCitizenInfomation(cocos2d::Sprite* Citizen,
                               const cocos2d::Vec2& InfoPosition,
                               int& priority);

    cocos2d::TMXTiledMap* tiled_map_{};
    std::unordered_map<std::string, cocos2d::TMXLayer*> map_layer_;
    cocos2d::TMXObjectGroup* object_group_{};
    cocos2d::Vec2 player_pos_;
    Avatar avatar{};

    std::map<Position, Crops*> CropPosition;
    std::map<Position, cocos2d::Sprite*> SpritePosition;
    std::vector<Crops*> MapCrops;
    std::vector<cocos2d::Sprite*> CropsSprite;
    std::vector<Animals*> MapAnimals;
    std::map<cocos2d::Sprite*, Animals*> SpritetoAnimal;
    std::vector<cocos2d::Sprite*> AnimalSprite;
    std::vector<Citizen*> MapCitizens;
    std::vector<cocos2d::Sprite*> CitizensSprite;
    std::map<cocos2d::Sprite*, Citizen*> SpritetoCitizen;
    static std::vector<Entity::Direction> AllDirection;
    std::string map_name_;

    int priority = 255;
    bool is_key_pressed_w_{false};
    bool is_key_pressed_a_{false};
    bool is_key_pressed_s_{false};
    bool is_key_pressed_d_{false};
    bool is_teleporting_{false};
};

}  // namespace th_valley

#endif  // TILED_MAP_H_
