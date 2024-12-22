#include "game/tiled_map.h"

#include <optional>
#include <regex>

#include "game/pig.h"
#include "math/CCGeometry.h"
#include "utility/logger.h"

namespace th_valley {

TiledMap::Portal::Portal(const std::string& portal_name) {
    std::regex portal_regex(R"((\w+)-(\w+))");
    std::smatch match;
    if (std::regex_match(portal_name, match, portal_regex)) {
        from_map_ = match[1];
        to_map_ = match[2];
    } else {
        Logger::GetInstance().LogError("Invalid portal name: {}", portal_name);
    }
}

std::string TiledMap::Portal::GetFromMap() const { return from_map_; }

std::string TiledMap::Portal::GetToMap() const { return to_map_; }

std::string TiledMap::Portal::GetPortalName() const {
    return from_map_ + "-" + to_map_;
}

std::string TiledMap::Portal::GetOppositePortalName() const {
    return to_map_ + "-" + from_map_;
}

TiledMap::Portal TiledMap::Portal::GetOppositePorta() const {
    return Portal(GetOppositePortalName());
}

TiledMap* TiledMap::create(const std::string& tmxFile) {
    TiledMap* ret = new (std::nothrow) TiledMap();
    if (ret && ret->InitWithTMXFile(tmxFile)) {
        ret->autorelease();
        return ret;
    } else {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

bool TiledMap::InitWithTMXFile(const std::string& tmxFile) {
    tiled_map_ = cocos2d::TMXTiledMap::create(tmxFile);
    if (!tiled_map_) {
        return false;
    }

    this->addChild(tiled_map_);

    tiled_map_->setAnchorPoint(cocos2d::Vec2(0, 0));
    tiled_map_->setPosition(cocos2d::Vec2(0, 0));

    std::vector<std::string> layerNames = {"Back",     "Back2",    "Block",
                                           "Interact", "Building", "Paths",
                                           "Front",    "Front2"};

    int zOrder = -5;
    for (const auto& name : layerNames) {
        auto layer = tiled_map_->getLayer(name);
        if (layer) {
            if (name == "Front") {
                zOrder += 5;
            }
            layer->setLocalZOrder(zOrder++);
            map_layer_[name] = layer;
            CCLOG("Layer '%s' added with z-order %d", name.c_str(), zOrder - 1);
        } else {
            CCLOG("Layer '%s' not found", name.c_str());
        }
    }

    object_group_ = tiled_map_->getObjectGroup("Objects");
    if (object_group_ == nullptr) {
        CCLOG("ObjectGroup 'Objects' not found");
    } else {
        CCLOG("ObjectGroup 'Objects' loaded successfully");
    }

    auto player = object_group_->getObject("Player");
    if (!player.empty()) {
        float x = player.at("x").asFloat();
        float y = player.at("y").asFloat();
        player_pos_ = cocos2d::Vec2(x, y);
    } else {
        CCLOG("Player object not found");
        player_pos_ = cocos2d::Vec2::ZERO;
    }
    CCLOG("Player position set to: %f, %f", player_pos_.x, player_pos_.y);
    CCLOG("Player position set to Tile: %f, %f",
          TileCoordFromPos(player_pos_).x, TileCoordFromPos(player_pos_).y);
    SetViewpointCenter(player_pos_);

    avatar.InitEntity(tiled_map_);
    avatar.setPosition(player_pos_);
    CCLOG("Player sprite created at %f %f", player_pos_.x, player_pos_.y);
    CCLOG("Player sprite created at Tile: %f %f",
          TileCoordFromPos(player_pos_).x, TileCoordFromPos(player_pos_).y);

    for (int pig_count = 0; pig_count < 10; pig_count++) {
        Pig* pig = new Pig;
        MapAnimals.push_back(pig);
    }
    initAnimalPosition();

    return true;
}

void TiledMap::CreateMiniMap() {
    auto* mini_map = cocos2d::Sprite::create();
    mini_map->setScale(0.1f);
    mini_map->setPosition(cocos2d::Vec2(500, 500));
    auto* player_marker = cocos2d::Sprite::create();
    player_marker->setColor(cocos2d::Color3B::RED);
    player_marker->setScale(0.2f);
    tiled_map_->addChild(mini_map, 100);
}

void TiledMap::Save() {
    // Implementation for save
}

void TiledMap::Load() {
    // Implementation for load
}

cocos2d::Rect TiledMap::GetPortalRect(Portal portal,
                                      std::string_view ObjectLayerName) {
    auto* object_group = tiled_map_->getObjectGroup(ObjectLayerName.data());
    if (object_group == nullptr) {
        Logger::GetInstance().LogError("ObjectGroup Objects not found");
        return cocos2d::Rect();
    }
    auto objects = object_group->getObjects();
    for (const auto& object : objects) {
        auto value_map = object.asValueMap();
        if (value_map["type"].asString() == "Portal") {
            std::string portal_name = value_map["name"].asString();
            if (portal_name == portal.GetPortalName()) {
                return cocos2d::Rect(value_map["x"].asFloat(),
                                     value_map["y"].asFloat(),
                                     value_map["width"].asFloat(),
                                     value_map["height"].asFloat());
            }
        }
    }
    Logger::GetInstance().LogError("Portal {} not found",
                                   portal.GetPortalName());
    return cocos2d::Rect();
}

void TiledMap::SetPlayerPos(cocos2d::Vec2 pos) {
    const auto map_size = tiled_map_->getMapSize();
    const auto tile_size = tiled_map_->getTileSize();
    const float map_width = map_size.width * tile_size.width;
    const float map_height = map_size.height * tile_size.height;

    pos.x = std::max(0.0F, std::min(pos.x, map_width));
    pos.y = std::max(0.0F, std::min(pos.y, map_height));

    player_pos_ = pos;
    Logger::GetInstance().LogInfo("Player position set to: {}, {}",
                                  player_pos_.x, player_pos_.y);
    Logger::GetInstance().LogInfo("Tile: {}, {}",
                                  TileCoordFromPos(player_pos_).x,
                                  TileCoordFromPos(player_pos_).y);
    SetViewpointCenter(player_pos_);
    avatar.setPosition(player_pos_);
}

void TiledMap::SetTeleportStatus(bool status) { is_teleporting_ = status; }

void TiledMap::onEnter() {
    cocos2d::Node::onEnter();
    CCLOG("Map onEnter");

    auto listener = cocos2d::EventListenerMouse::create();

    listener->onMouseMove = [this](cocos2d::Event* event) {
        auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
        auto pos =
            tiled_map_->convertToNodeSpace(mouseEvent->getLocationInView());
        auto tilePos = TileCoordFromPos(pos);
    };

    auto keyListener = cocos2d::EventListenerKeyboard::create();
    keyListener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode,
                                       cocos2d::Event* event) {
        switch (keyCode) {
            case cocos2d::EventKeyboard::KeyCode::KEY_W:
                is_key_pressed_w_ = true;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                is_key_pressed_s_ = true;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                is_key_pressed_a_ = true;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                is_key_pressed_d_ = true;
                break;
            default:
                break;
        }
    };

    keyListener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode,
                                        cocos2d::Event* event) {
        switch (keyCode) {
            case cocos2d::EventKeyboard::KeyCode::KEY_W:
                is_key_pressed_w_ = false;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                is_key_pressed_s_ = false;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                is_key_pressed_a_ = false;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                is_key_pressed_d_ = false;
                break;
            default:
                break;
        }
    };

    cocos2d::EventDispatcher* dispatcher =
        cocos2d::Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    this->scheduleUpdate();
}

void TiledMap::update(const float delta) {
    cocos2d::Vec2 current_pos = GetPos();
    const float move_step = 100.0F * delta;
    auto portal = GetPortal(current_pos);
    if (portal.has_value()) {
        if (!is_teleporting_) {
            MapController::GetInstance().TriggerTeleport(
                portal->GetPortalName());
            is_teleporting_ = true;
            return;
        }
    } else {
        SetTeleportStatus(false);
    }

    if (is_key_pressed_w_) {
        current_pos.y += move_step;
        avatar.ChangeDirection(Entity::Direction::kUp);
    }
    if (is_key_pressed_s_) {
        current_pos.y -= move_step;
        avatar.ChangeDirection(Entity::Direction::kDown);
    }
    if (is_key_pressed_a_) {
        current_pos.x -= move_step;
        avatar.ChangeDirection(Entity::Direction::kLeft);
    }
    if (is_key_pressed_d_) {
        current_pos.x += move_step;
        avatar.ChangeDirection(Entity::Direction::kRight);
    }

    if (is_key_pressed_w_ || is_key_pressed_s_ || is_key_pressed_a_ ||
        is_key_pressed_d_) {
        if (IsCollisionAtAnyLayer(current_pos)) {
            Logger::GetInstance().LogInfo("Collision detected");
        } else {
            SetPlayerPos(current_pos);
        }
    }
}

bool TiledMap::IsCollision(cocos2d::Vec2 pos, std::string LayerName) {
    auto Layer = tiled_map_->getLayer(LayerName);

    auto tileCoord = this->TileCoordFromPos(pos);

    int tileGid = Layer->getTileGIDAt(tileCoord);
    if (tileGid != 0) {
        auto properties = tiled_map_->getPropertiesForGID(tileGid);

        if (properties.getType() == cocos2d::Value::Type::MAP) {
            auto valueMap = properties.asValueMap();

            auto it = valueMap.find("Collidable");
            if (it != valueMap.end() && it->second.asString() == "true") {
                CCLOG("Collision detected at tile: %f, %f at Layer %s",
                      tileCoord.x, tileCoord.y, LayerName.c_str());
                return true;
            }
        }
    }
    return false;
}

bool TiledMap::IsCollisionAtAnyLayer(cocos2d::Vec2 pos) {
    for (const auto& pair : map_layer_) {
        if (IsCollision(pos, pair.first)) {
            return true;
        }
    }
    return false;
}

cocos2d::Vec2 TiledMap::GetPos() { return player_pos_; }

cocos2d::Vec2 TiledMap::TileCoordFromPos(cocos2d::Vec2 pos) {
    int x = pos.x / tiled_map_->getTileSize().width;
    int y =
        (tiled_map_->getMapSize().height * tiled_map_->getTileSize().height -
         pos.y) /
        tiled_map_->getTileSize().height;
    return cocos2d::Vec2(x, y);
}

cocos2d::Vec2 TiledMap::PosFromtileCoord(Position PlantPosition) {
    cocos2d::Vec2 PicturePosition;
    PicturePosition.x = PlantPosition.x * tiled_map_->getTileSize().width;
    PicturePosition.y =
        (tiled_map_->getMapSize().height * tiled_map_->getTileSize().height) -
        PlantPosition.y * tiled_map_->getTileSize().height;
    PicturePosition.x += tiled_map_->getTileSize().width / 2;
    PicturePosition.y -= tiled_map_->getTileSize().height / 2;
    return PicturePosition;
}

int TiledMap::GetTileID(cocos2d::Vec2 tileCoord, std::string LayerName) {
    auto layer = tiled_map_->getLayer(LayerName);
    int gid = layer->getTileGIDAt(tileCoord);
    if (gid == 0) {
        CCLOG("No tile at this position\n");
        return 0;
    }
    return gid;
}

bool TiledMap::PropertyCheck(int gid, std::string property) {
    cocos2d::Value properties = tiled_map_->getPropertiesForGID(gid);
    if (properties.isNull()) {
        CCLOG("There is no properties\n");
        return false;
    } else {
        cocos2d::ValueMap propertyMap = properties.asValueMap();
        auto it = propertyMap.find(property);
        if (it != propertyMap.end()) {
            bool isCorrect = it->second.asBool();
            if (isCorrect) {
                CCLOG("%s is true\n", property.c_str());
                return true;
            } else {
                CCLOG("%s is false\n", property.c_str());
                return false;
            }
        }
        CCLOG("Not find property: %s\n", property.c_str());
        return false;
    }
}

std::optional<TiledMap::Portal> TiledMap::GetPortal(
    cocos2d::Vec2 pos, std::string_view ObjectLayerName) const {
    auto* object_group = tiled_map_->getObjectGroup(ObjectLayerName.data());
    if (object_group == nullptr) {
        Logger::GetInstance().LogError("ObjectGroup {} not found",
                                       ObjectLayerName);
        return std::nullopt;
    }
    auto objects = object_group->getObjects();
    for (const auto& object : objects) {
        auto value_map = object.asValueMap();
        if (value_map["type"].asString() == "Portal") {
            auto rect = cocos2d::Rect(
                value_map["x"].asFloat(), value_map["y"].asFloat(),
                value_map["width"].asFloat(), value_map["height"].asFloat());
            if (rect.containsPoint(pos)) {
                return Portal(value_map["name"].asString());
            }
        }
    }
    return std::nullopt;
}

void TiledMap::SetViewpointCenter(cocos2d::Vec2 pos) {
    auto mapSize = tiled_map_->getMapSize();
    auto tileSize = tiled_map_->getTileSize();
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    float x = MAX(pos.x, visibleSize.width / 2);
    float y = MAX(pos.y, visibleSize.height / 2);
    x = MIN(x, (mapSize.width * tileSize.width) - visibleSize.width / 2);
    y = MIN(y, (mapSize.height * tileSize.height) - visibleSize.height / 2);

    cocos2d::Vec2 centerPoint =
        cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2);
    cocos2d::Vec2 actualPoint = cocos2d::Vec2(x, y);
    CCLOG("actualPoint: %f, %f", actualPoint.x, actualPoint.y);

    cocos2d::Vec2 offset = centerPoint - actualPoint;

    CCLOG("offset: %f, %f", offset.x, offset.y);

    if (mapSize.width * tileSize.width < visibleSize.width) {
        offset.x = (visibleSize.width - mapSize.width * tileSize.width) / 2;
    }
    if (mapSize.height * tileSize.height < visibleSize.height) {
        offset.y = (visibleSize.height - mapSize.height * tileSize.height) / 2;
    }

    tiled_map_->setPosition(offset);
}

void TiledMap::UpdateTileAt(cocos2d::Vec2 tileCoord, int newGID,
                            std::string LayerName) {
    auto layer = tiled_map_->getLayer(LayerName);
    layer->setTileGID(newGID, tileCoord);
    CCLOG("Tile updated at %f, %f with GID: %d", tileCoord.x, tileCoord.y,
          newGID);
}

}  // namespace th_valley
