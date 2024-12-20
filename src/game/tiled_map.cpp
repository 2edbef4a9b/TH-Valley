#include "game/tiled_map.h"

namespace th_valley {

bool TiledMap::initWithTMXFile(const std::string& tmxFile) {
    if (!cocos2d::TMXTiledMap::initWithTMXFile(tmxFile)) {
        return false;
    }

    tileMap_ = this;  // Assign the current instance to tileMap_
    this->setAnchorPoint(cocos2d::Vec2(0, 0));
    this->setPosition(cocos2d::Vec2(0, 0));

    std::vector<std::string> layerNames = {"Back",     "Back2",    "Block",
                                           "Interact", "Building", "Paths",
                                           "Front",    "Front2"};

    int zOrder = -5;
    for (const auto& name : layerNames) {
        auto layer = tileMap_->getLayer(name);
        if (layer) {
            if (name == "Front") {
                zOrder += 5;
            }
            layer->setLocalZOrder(zOrder++);
            mapLayer_[name] = layer;
            CCLOG("Layer '%s' added with z-order %d", name.c_str(), zOrder - 1);
        } else {
            CCLOG("Layer '%s' not found", name.c_str());
        }
    }

    objectGroup_ = tileMap_->getObjectGroup("Objects");
    if (objectGroup_ == nullptr) {
        CCLOG("ObjectGroup 'Objects' not found");
    } else {
        CCLOG("ObjectGroup 'Objects' loaded successfully");
    }

    // ��ȡ��ʼ���λ�ￄ1�7
    auto player = objectGroup_->getObject("Player");
    if (!player.empty()) {
        float x = player.at("x").asFloat();
        float y = player.at("y").asFloat();
        playerPos_ = cocos2d::Vec2(x, y);

    } else {
        CCLOG("Player object not found");
        playerPos_ = cocos2d::Vec2::ZERO;
    }
    CCLOG("Player position set to: %f, %f", playerPos_.x, playerPos_.y);
    CCLOG("Player position set to Tile: %f, %f", tileCoordFromPos(playerPos_).x,
          tileCoordFromPos(playerPos_).y);
    setViewpointCenter(playerPos_);

    auto texture =
        cocos2d::Director::getInstance()->getTextureCache()->addImage(
            "assets/Sebastian.png");

    cocos2d::Rect frameRect(0, 0, 16, 32);
    auto spriteFrame =
        cocos2d::SpriteFrame::createWithTexture(texture, frameRect);
    playerSprite_ = cocos2d::Sprite::createWithSpriteFrame(spriteFrame);
    playerSprite_->setAnchorPoint(cocos2d::Vec2(0.5f, 0.0f));
    playerSprite_->setPosition(playerPos_);
    CCLOG("Player sprite created at %f %f", playerPos_.x, playerPos_.y);
    CCLOG("Player sprite created at Tile: %f %f", tileCoordFromPos(playerPos_).x,
          tileCoordFromPos(playerPos_).y);
    this->addChild(playerSprite_, 2);

    return true;
}

TiledMap* TiledMap::create(const std::string& tmxFile) {
    TiledMap* ret = new (std::nothrow) TiledMap();
    if (ret && ret->initWithTMXFile(tmxFile)) {
        ret->autorelease();
        return ret;
    } else {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

void TiledMap::setPlayerPos(cocos2d::Vec2 pos) {
    auto mapSize = this->getMapSize();
    auto tileSize = this->getTileSize();
    float mapWidth = mapSize.width * tileSize.width;
    float mapHeight = mapSize.height * tileSize.height;

    pos.x = std::max(0.0f, std::min(pos.x, mapWidth));
    pos.y = std::max(0.0f, std::min(pos.y, mapHeight));

    if (isCollisionAtAnyLayer(pos)) {
        CCLOG("Collision detected, player position not updated");
    } else {
        playerPos_ = pos;
        CCLOG("Player position set to: %f, %f", pos.x, pos.y);
        CCLOG("Tile: %f, %f", tileCoordFromPos(pos).x, tileCoordFromPos(pos).y);

        setViewpointCenter(pos);

        playerSprite_->setPosition(playerPos_);
    }
}

cocos2d::Vec2 TiledMap::tileCoordFromPos(cocos2d::Vec2 pos) {
    // pos = this->convertToNodeSpace(pos);
    int x = pos.x / tileMap_->getTileSize().width;
    int y =
        (tileMap_->getMapSize().height * tileMap_->getTileSize().height - pos.y) /
        tileMap_->getTileSize().height;
    return cocos2d::Vec2(x, y);
}

void TiledMap::setViewpointCenter(cocos2d::Vec2 pos) {
    auto mapSize = this->getMapSize();
    auto tileSize = this->getTileSize();
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

    this->setPosition(offset);
}

bool TiledMap::isCollision(cocos2d::Vec2 pos, std::string LayerName) {
    auto Layer = this->getLayer(LayerName);

    auto tileCoord = this->tileCoordFromPos(pos);

    int tileGid = Layer->getTileGIDAt(tileCoord);
    if (tileGid != 0) {
        auto properties = this->getPropertiesForGID(tileGid);

        if (properties.getType() == cocos2d::Value::Type::MAP) {
            auto valueMap = properties.asValueMap();

            auto it = valueMap.find("Collidable");
            if (it != valueMap.end() && it->second.asString() == "true") {
                CCLOG("Collision detected at tile: %f, %f at Layer %s",
                      tileCoord.x, tileCoord.y, LayerName.c_str());
                return true;
            }

            // auto it2 = valueMap.find("Cultivable");
            // if (it2 != valueMap.end() && it2->second.asString() == "true") {
            //     CCLOG("LAND detected at tile: %f, %f at Layer %s",
            //           tileCoord.x, tileCoord.y, LayerName.c_str());
            //     updateTileAt(tileCoord, 557, LayerName);
            // }
        }
    }
    return false;
}

bool TiledMap::isCollisionAtAnyLayer(cocos2d::Vec2 pos) {
    for (const auto& pair : mapLayer_) {
        if (isCollision(pos, pair.first)) {
            return true;
        }
    }
    return false;
}

bool TiledMap::isPortal(cocos2d::Vec2 pos, std::string ObjectLayerName) {
    auto objectGroup_ = this->getObjectGroup(ObjectLayerName);
    if (objectGroup_ == nullptr) {
        CCLOG("ObjectGroup %s not found", ObjectLayerName.c_str());
        return false;
    }
    auto objects = objectGroup_->getObjects();
    for (const auto& obj : objects) {
        auto dict = obj.asValueMap();
        if (dict["type"].asString() == "Portal") {
            auto rect = cocos2d::Rect(dict["x"].asFloat(), dict["y"].asFloat(),
                                      dict["width"].asFloat(),
                                      dict["height"].asFloat());
            CCLOG("Checking portal at rect: %f, %f, %f, %f", rect.origin.x,
                  rect.origin.y, rect.size.width, rect.size.height);
            if (rect.containsPoint(pos)) {
                CCLOG("Portal detected at tile: %f, %f", pos.x, pos.y);
                // ���ݾ���������ɲ�ͬ����ͼ���ￄ1�7
                std::string portalName = dict["name"].asString();
                CCLOG("Portal %s detected.", portalName.c_str());
                triggerPortalEvent(portalName);
                return true;
            }
        }
    }
    return false;
}

void TiledMap::triggerPortalEvent(const std::string& portalName) {
    if (portalName == "FarmToHome") {
        CCLOG("Triggering %s event", portalName.c_str());
        auto house = TiledMap::create("assets/maps/FarmHouse.tmx");
        if (house) {
            house->setPosition(cocos2d::Vec2(200, 200));
            house->setVisible(true);
            this->addChild(house, 12);
        } else
            CCLOG("Failed to load map maps/FarmHouse.tmx");
    } else {
        CCLOG("Unknown portal: %s", portalName.c_str());
    }
}

void TiledMap::checkEventsAndTrigger(cocos2d::Vec2 tileCoord) {
    //// ��ȡ��Ƭ�����Ӧ��gid
    // int gid = collisionLayer->getTileGIDAt(tileCoord);
    // if (gid) {
    //     auto properties = this->getPropertiesForGID(gid).asValueMap();
    //     if (properties["Event"].asString() == "True") {
    //         // �����¼�
    //         CCLOG("Event triggered at tile: %f, %f", tileCoord.x,
    //         tileCoord.y);
    //         // �����¼�����ִ����Ӧ���߼�
    //         // ���磺this->triggerBattleEvent();
    //     }
    // }
}

cocos2d::Vec2 TiledMap::getPos() { return playerPos_; }

void TiledMap::updateTileAt(cocos2d::Vec2 tileCoord, int newGID,
                            std::string LayerName) {
    auto layer = tileMap_->getLayer(LayerName);
    layer->setTileGID(newGID, tileCoord);
    CCLOG("Tile updated at %f, %f with GID: %d", tileCoord.x, tileCoord.y,
          newGID);
}

void TiledMap::onEnter() {
    cocos2d::TMXTiledMap::onEnter();
    CCLOG("Map onEnter");

    auto listener = cocos2d::EventListenerMouse::create();

    listener->onMouseDown = [this](cocos2d::Event* event) {
        auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
        auto pos = this->convertToNodeSpace(mouseEvent->getLocationInView());
        auto tilePos = tileCoordFromPos(pos);

        CCLOG("Mouse Down Tile: %f, %f", tilePos.x, tilePos.y);
        CCLOG("Mouse Down: %f, %f", pos.x, pos.y);

        for (const auto& layer : mapLayer_) {
            if (layer.second != nullptr) {
                int gid = layer.second->getTileGIDAt(tilePos);
                CCLOG("Checking layer: %s, gid: %d", layer.first.c_str(), gid);
                isCollision(pos, layer.first);
            }
        }
        isPortal(pos);
    };

    listener->onMouseMove = [this](cocos2d::Event* event) {
        auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
        auto pos = this->convertToNodeSpace(mouseEvent->getLocationInView());
        auto tilePos = tileCoordFromPos(pos);
        // CCLOG("Mouse Move Tile: %f, %f", tilePos.x, tilePos.y);
        // CCLOG("Mouse Move: %f, %f", pos.x, pos.y);
    };

    auto keyListener = cocos2d::EventListenerKeyboard::create();
    keyListener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode,
                                       cocos2d::Event* event) {
        switch (keyCode) {
            case cocos2d::EventKeyboard::KeyCode::KEY_W:
                isKeyPressedW_ = true;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                isKeyPressedS_ = true;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                isKeyPressedA_ = true;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                isKeyPressedD_ = true;
                break;
            default:
                break;
        }
    };

    keyListener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode,
                                        cocos2d::Event* event) {
        switch (keyCode) {
            case cocos2d::EventKeyboard::KeyCode::KEY_W:
                isKeyPressedW_ = false;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                isKeyPressedS_ = false;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                isKeyPressedA_ = false;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                isKeyPressedD_ = false;
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

void TiledMap::createMiniMap() {
    auto * mini_map = cocos2d::Sprite::create();
    mini_map->setScale(0.1f);
    mini_map->setPosition(cocos2d::Vec2(500, 500));
    auto * player_marker = cocos2d::Sprite::create();
    player_marker->setColor(cocos2d::Color3B::RED);
    player_marker->setScale(0.2f);
    this->addChild(mini_map, 100);
}

void TiledMap::save() {
}

void TiledMap::load() {
}

void TiledMap::update(float delta) {
    cocos2d::Vec2 current_pos = getPos();
    const float move_step =
        100.0f * delta;

    if (isKeyPressedW_) {
        current_pos.y += move_step;
    }
    if (isKeyPressedS_) {
        current_pos.y -= move_step;
    }
    if (isKeyPressedA_) {
        current_pos.x -= move_step;
    }
    if (isKeyPressedD_) {
        current_pos.x += move_step;
    }

    if (isKeyPressedW_ || isKeyPressedS_ || isKeyPressedA_ || isKeyPressedD_) {
        setPlayerPos(current_pos);
    }
}

}  // namespace th_valley
