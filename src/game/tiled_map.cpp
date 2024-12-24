#include "game/tiled_map.h"

#include <cstdio>
#include <fstream>
#include <optional>
#include <regex>
#include <sstream>

#include "json/document.h"
#include "json/filereadstream.h"
#include "json/filewritestream.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

// ...existing code...
#include "client/client_controller.h"
#include "frontend/game_scene.h"
#include "game/crop_production.h"
#include "game/pig.h"
#include "game/vegetables.h"
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

void TiledMap::SpawnAnimal(int count) {
    for (int pig_count = 0; pig_count < count; pig_count++) {
        Pig* pig = new Pig;
        MapAnimals.push_back(pig);
    }
    initAnimalPosition();
}

void TiledMap::SpawnCitizen(Citizen* citizen) {
    MapCitizens.push_back(citizen);
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
    // Logger::GetInstance().LogInfo("Player position set to: {}, {}",
    //                               player_pos_.x, player_pos_.y);
    // Logger::GetInstance().LogInfo("Tile: {}, {}",
    //                               TileCoordFromPos(player_pos_).x,
    //                               TileCoordFromPos(player_pos_).y);
    SetViewpointCenter(player_pos_);
    avatar.setPosition(player_pos_);
}

void TiledMap::SetTeleportStatus(bool status) { is_teleporting_ = status; }

void TiledMap::onEnter() {
    cocos2d::Node::onEnter();
    CCLOG("Map onEnter");

    auto listener = cocos2d::EventListenerMouse::create();

    listener->onMouseDown = [this](cocos2d::Event* event) {
        auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
        auto pos =
            tiled_map_->convertToNodeSpace(mouseEvent->getLocationInView());
        auto tilePos = TileCoordFromPos(pos);
        CCLOG("Mouse Down Tile: %f, %f", tilePos.x, tilePos.y);
        CCLOG("Mouse Down: %f, %f", pos.x, pos.y);

        for (const auto& layer : map_layer_) {
            if (layer.second != nullptr) {
                int gid = layer.second->getTileGIDAt(tilePos);
                CCLOG("Checking layer: %s, gid: %d", layer.first.c_str(), gid);
                IsCollision(pos, layer.first);
            }
        }

        int gid = GetTileID(tilePos, "Back");
        auto MapToolBar =
            dynamic_cast<GameScene*>(this->getParent())->GetToolBar();
        if (PropertyCheck(gid, "Cultivable") &&
            MapToolBar->getToolName() == "Hoe") {
            if (MapToolBar != nullptr)
                MapToolBar->outputindex();
            else
                CCLOG("MapToolBar is nullptr");

            auto PlayerTilePos = TileCoordFromPos(GetPos());
            CCLOG("player_pos_: %f %f", GetPos().x, GetPos().y);
            CCLOG("tile_pos_: %f %f", tilePos.x, tilePos.y);
            if (fabs(PlayerTilePos.x - tilePos.x) > 1 ||
                fabs(PlayerTilePos.y - tilePos.y) > 1) {
                CCLOG("Too far to cultivate");
            } else {
                UpdateTileAt(tilePos, 557, "Back");
                avatar.UseTool("Hoe");
                CCLOG("Tile is cultivable\n");
            }
        } else {
            Position PlantTilePos;
            PlantTilePos.x = tilePos.x;
            PlantTilePos.y = tilePos.y;

            if (MapToolBar->getToolName() == "StrawberrySeed") {
                CropPlant(PlantTilePos, new Strawberry);
            } else if (MapToolBar->getToolName() == "CarrotSeed") {
                CropPlant(PlantTilePos, new Carrot);
            } else if (MapToolBar->getToolName() == "PotatoSeed") {
                CropPlant(PlantTilePos, new Potato);
            }
        }

        // Animal
        float MinDistance = 99999999;
        cocos2d::Sprite* Closest;
        for (int i = 0; i < AnimalSprite.size(); i++) {
            auto SpritePosition = AnimalSprite[i]->getPosition();
            float Distance =
                sqrt((SpritePosition.x - pos.x) * (SpritePosition.x - pos.x) +
                     (SpritePosition.y - pos.y) * (SpritePosition.y - pos.y));
            CCLOG("Animal Position: %f %f\n", SpritePosition.x,
                  SpritePosition.y);
            if (fabs(SpritePosition.x - pos.x) < 20 &&
                fabs(SpritePosition.y - pos.y) < 20 && Distance < MinDistance) {
                CCLOG("Sprite clicked!");
                MinDistance = Distance;
                Closest = AnimalSprite[i];
            }
        }

        if (MinDistance < 99999999 && !SpritetoAnimal[Closest]->InfoOpen) {
            SpritetoAnimal[Closest]->InfoOpen = true;
            ShowAnimalInfomation(Closest, pos, priority);
        }

        // Citizen
        MinDistance = 99999999;
        cocos2d::Sprite* ClosestCitizen;
        for (int i = 0; i < CitizensSprite.size(); i++) {
            auto SpritePosition = CitizensSprite[i]->getPosition();
            float Distance =
                sqrt((SpritePosition.x - pos.x) * (SpritePosition.x - pos.x) +
                     (SpritePosition.y - pos.y) * (SpritePosition.y - pos.y));
            CCLOG("Citizen Position: %f %f\n", SpritePosition.x,
                  SpritePosition.y);
            if (fabs(SpritePosition.x - pos.x) < 20 &&
                fabs(SpritePosition.y - pos.y) < 20 && Distance < MinDistance) {
                CCLOG("Sprite clicked!");
                MinDistance = Distance;
                ClosestCitizen = CitizensSprite[i];
            }
        }

        if (MinDistance < 99999999 &&
            !SpritetoCitizen[ClosestCitizen]->InfoOpen) {
            if (SpritetoCitizen[ClosestCitizen]->CitizenName == "Sebastian") {
                SpritetoCitizen[ClosestCitizen]->ShowTalkBox(
                    this->getParent(), MapToolBar->bag_,
                    SpritetoCitizen[ClosestCitizen]->CitizenName);
            } else {
                if (MapToolBar->getToolName() == "Strawberry") {
                    MapToolBar->bag_->ReduceItem(MapToolBar->selectedToolIndex);
                    MapToolBar->bag_->EarnMoney(140);
                }
                if (MapToolBar->getToolName() == "Potato") {
                    MapToolBar->bag_->ReduceItem(MapToolBar->selectedToolIndex);
                    MapToolBar->bag_->EarnMoney(70);
                }
                if (MapToolBar->getToolName() == "Carrot") {
                    MapToolBar->bag_->ReduceItem(MapToolBar->selectedToolIndex);
                    MapToolBar->bag_->EarnMoney(70);
                }
            }
        }
        MapToolBar->loadTools();
    };

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
            case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
                is_key_pressed_w_ = true;
                // avatar.ChangeDirection(Entity::Direction::kUp);
                AllDirection.push_back(Entity::Direction::kUp);
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
            case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
                is_key_pressed_s_ = true;
                // avatar.ChangeDirection(Entity::Direction::kDown);
                AllDirection.push_back(Entity::Direction::kDown);
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
            case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
                is_key_pressed_a_ = true;
                // avatar.ChangeDirection(Entity::Direction::kLeft);
                AllDirection.push_back(Entity::Direction::kLeft);
                Logger::GetInstance().LogInfo("A key pressed");
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
            case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
                is_key_pressed_d_ = true;
                // avatar.ChangeDirection(Entity::Direction::kRight);
                AllDirection.push_back(Entity::Direction::kRight);
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_E:
            case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_E:
                if (tiled_map_->getParent()) {
                    avatar.UseTool(dynamic_cast<GameScene*>(this->getParent())
                                       ->GetToolBar()
                                       ->getToolName());
                } else {
                    Logger::GetInstance().LogError("Parent node is null");
                }
                break;
            /*case cocos2d::EventKeyboard::KeyCode::KEY_F:
            case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_F:
                if (tiled_map_->getParent()) {
                    avatar.Attacking(dynamic_cast<GameScene*>(this->getParent())->GetToolBar()
                                       ->getToolName());
                } else {
                    Logger::GetInstance().LogError("Parent node is null");
                }*/
            case cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE:
                SavePlayerInfo();
                ClientController::GetInstance().SetClientState(
                    ClientController::ClientState::kTitleScreen);
                break;
            default:
                Logger::GetInstance().LogError("!");
                break;
        }
        if (is_key_pressed_a_ || is_key_pressed_d_ || is_key_pressed_s_ ||
            is_key_pressed_w_) {
            avatar.ChangeDirection(AllDirection[0]);
            avatar.RenderMove();
        }
    };

    keyListener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode,
                                        cocos2d::Event* event) {
        std::vector<Entity::Direction>::iterator iterator;
        switch (keyCode) {
            case cocos2d::EventKeyboard::KeyCode::KEY_W:
            case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
                is_key_pressed_w_ = false;
                for (iterator = AllDirection.begin();
                     iterator != AllDirection.end(); iterator++) {
                    if (*iterator == Entity::Direction::kUp) {
                        break;
                    }
                }
                AllDirection.erase(iterator);
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
            case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
                is_key_pressed_s_ = false;
                for (iterator = AllDirection.begin();
                     iterator != AllDirection.end(); iterator++) {
                    if (*iterator == Entity::Direction::kDown) {
                        break;
                    }
                }
                AllDirection.erase(iterator);
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
            case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
                is_key_pressed_a_ = false;
                for (iterator = AllDirection.begin();
                     iterator != AllDirection.end(); iterator++) {
                    if (*iterator == Entity::Direction::kLeft) {
                        break;
                    }
                }
                AllDirection.erase(iterator);
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
            case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
                is_key_pressed_d_ = false;
                for (iterator = AllDirection.begin();
                     iterator != AllDirection.end(); iterator++) {
                    if (*iterator == Entity::Direction::kRight) {
                        break;
                    }
                }
                AllDirection.erase(iterator);
                break;
            default:
                break;
        }
        avatar.stopAllActions();
        if (AllDirection.size() == 0) {
            avatar.SetState(Entity::EntityState::kIdle);
        } else {
            avatar.ChangeDirection(AllDirection[0]);
            avatar.RenderMove();
        }
    };

    cocos2d::EventDispatcher* dispatcher =
        cocos2d::Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    this->scheduleUpdate();
    avatar.scheduleUpdate();
}

void TiledMap::update(const float delta) {
    cocos2d::Vec2 current_pos = GetPos();
    const float move_step = 80.0F * delta;
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
    }
    if (is_key_pressed_s_) {
        current_pos.y -= move_step;
    }
    if (is_key_pressed_a_) {
        current_pos.x -= move_step;
    }
    if (is_key_pressed_d_) {
        current_pos.x += move_step;
    }

    // CCLOG("current_pos: %d %d", current_pos.x, current_pos.y);
    if (is_key_pressed_w_ || is_key_pressed_s_ || is_key_pressed_a_ ||
        is_key_pressed_d_) {
        if (IsCollisionAtAnyLayer(current_pos)) {
            Logger::GetInstance().LogInfo("Collision detected");
            avatar.SetState(Entity::EntityState::kIdle);
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
    // CCLOG("actualPoint: %f, %f", actualPoint.x, actualPoint.y);

    cocos2d::Vec2 offset = centerPoint - actualPoint;

    // CCLOG("offset: %f, %f", offset.x, offset.y);

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

void TiledMap::Save(const std::string& file_name) {
    Logger::GetInstance().LogInfo("Saving game state to: {}", file_name);

    std::string save_directory =
        cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath() +
        "saves/";
    std::string save_path = save_directory + file_name;

    // Create the save directory if it doesn't exist
    cocos2d::FileUtils::getInstance()->createDirectory(save_directory);

    // Create a JSON document
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    // Serialize crops
    rapidjson::Value crops_array(rapidjson::kArrayType);
    for (const auto& crop_pair : CropPosition) {
        const Position& pos = crop_pair.first;
        Crops* crop = crop_pair.second;

        rapidjson::Value crop_object(rapidjson::kObjectType);
        crop_object.AddMember("x", pos.x, allocator);
        crop_object.AddMember("y", pos.y, allocator);

        rapidjson::Value crop_type_value;
        if (crop == nullptr) {
            crop_type_value.SetString("Removed", allocator);
        } else {
            crop_type_value.SetString(crop->getCropName().c_str(), allocator);
        }
        crop_object.AddMember("type", crop_type_value, allocator);

        rapidjson::Value crop_stage_value;
        if (crop == nullptr) {
            crop_stage_value.SetInt(-1);
        } else {
            crop_stage_value.SetInt(crop->CurrentGrowthStage);
        }

        crop_object.AddMember("stage", crop_stage_value, allocator);

        crops_array.PushBack(crop_object, allocator);
    }
    document.AddMember("crops", crops_array, allocator);

    // Write the JSON document to a file
    FILE* fp = fopen(save_path.c_str(), "wb");
    if (fp) {
        char writeBuffer[65536];
        rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
        rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
        document.Accept(writer);
        fclose(fp);
    } else {
        Logger::GetInstance().LogError("Failed to open file for saving: {}",
                                       save_path);
    }

    while (GlobalCropProduction.AllCrops.size() > 0) {
        delete GlobalCropProduction.AllCrops.back();
        GlobalCropProduction.AllCrops.pop_back();
    }
}

void TiledMap::Load(const std::string& file_name) {
    Logger::GetInstance().LogInfo("Loading game state from: {}", file_name);
    std::string save_directory =
        cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath() +
        "saves/";
    std::string save_path = save_directory + file_name;

    // Open the JSON file
    FILE* fp = fopen(save_path.c_str(), "rb");
    if (fp) {
        char readBuffer[65536];
        rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        rapidjson::Document document;
        document.ParseStream(is);
        fclose(fp);

        if (document.HasParseError()) {
            Logger::GetInstance().LogError("Failed to parse JSON file: {}",
                                           save_path);
            return;
        }

        // Deserialize crops
        if (document.HasMember("crops") && document["crops"].IsArray()) {
            const rapidjson::Value& crops_array = document["crops"];
            for (rapidjson::SizeType i = 0; i < crops_array.Size(); ++i) {
                const rapidjson::Value& crop_object = crops_array[i];
                if (crop_object.IsObject()) {
                    Position pos;
                    pos.x = crop_object["x"].GetFloat();
                    pos.y = crop_object["y"].GetFloat();
                    std::string crop_type = crop_object["type"].GetString();
                    int crop_stage = crop_object["stage"].GetInt();

                    Crops* crop = nullptr;
                    bool is_removed = crop_type == "Removed";
                    if (crop_type == "Strawberry") {
                        crop = new Strawberry();
                    } else if (crop_type == "Carrot") {
                        crop = new Carrot();
                    } else if (crop_type == "Potato") {
                        crop = new Potato();
                    } else {
                        Logger::GetInstance().LogError("Unknown crop type: {}",
                                                       crop_type);
                    }

                    if (crop && !is_removed) {
                        crop->setCurrentGrowthStage(crop_stage);
                        CropPosition[pos] = crop;
                        MapCrops.push_back(crop);
                        crop->CropSprite->setPosition(PosFromtileCoord(pos));
                        tiled_map_->addChild(crop->CropSprite, 4);
                        UpdateTileAt(cocos2d::Vec2(pos.x, pos.y), 557, "Back");
                        GlobalCropProduction.AllCrops.push_back(crop);
                        SpritePosition[pos] = crop->CropSprite;
                    } else if (is_removed) {
                        UpdateTileAt(cocos2d::Vec2(pos.x, pos.y), 557, "Back");
                    }
                }
            }
        }
    } else {
        Logger::GetInstance().LogError("Failed to open file for loading: {}",
                                       save_path);
    }
}

void TiledMap::SavePlayerInfo() {
    std::string save_path =
        cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath() +
        "saves/player_state.json";

    // Create a JSON document
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    // Add map name
    document.AddMember(
        "map_name", rapidjson::Value(map_name_.c_str(), allocator), allocator);

    // Add player position
    rapidjson::Value player_pos(rapidjson::kObjectType);
    player_pos.AddMember("x", player_pos_.x, allocator);
    player_pos.AddMember("y", player_pos_.y, allocator);
    document.AddMember("player_pos", player_pos, allocator);

    // Write the JSON document to a file
    FILE* fp = fopen(save_path.c_str(), "wb");
    if (fp) {
        char writeBuffer[65536];
        rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
        rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
        document.Accept(writer);
        fclose(fp);
    } else {
        Logger::GetInstance().LogError("Failed to open file for saving: {}",
                                       save_path);
    }
}

void TiledMap::LoadPlayerInfo() {
    std::string save_path =
        cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath() +
        "saves/player_state.json";

    // Open the JSON file
    FILE* fp = fopen(save_path.c_str(), "rb");
    if (fp) {
        char readBuffer[65536];
        rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        rapidjson::Document document;
        document.ParseStream(is);
        fclose(fp);

        if (document.HasParseError()) {
            Logger::GetInstance().LogError("Failed to parse JSON file: {}",
                                           save_path);
            return;
        }

        // Load map name
        if (document.HasMember("map_name") && document["map_name"].IsString()) {
            map_name_ = document["map_name"].GetString();
        } else {
            Logger::GetInstance().LogError(
                "Map name not found in JSON file: {}", save_path);
        }

        // Load player position
        if (document.HasMember("player_pos") &&
            document["player_pos"].IsObject()) {
            const rapidjson::Value& player_pos = document["player_pos"];
            if (player_pos.HasMember("x") && player_pos["x"].IsFloat() &&
                player_pos.HasMember("y") && player_pos["y"].IsFloat()) {
                player_pos_.x = player_pos["x"].GetFloat();
                player_pos_.y = player_pos["y"].GetFloat();
            } else {
                Logger::GetInstance().LogError(
                    "Player position not found in JSON file: {}", save_path);
            }
        } else {
            Logger::GetInstance().LogError(
                "Player position not found in JSON file: {}", save_path);
        }
    } else {
        Logger::GetInstance().LogError("Failed to open file for loading: {}",
                                       save_path);
    }
}

void TiledMap::SetMapName(const std::string& map_name) { map_name_ = map_name; }

std::vector<Entity::Direction> TiledMap::AllDirection;

}  // namespace th_valley
