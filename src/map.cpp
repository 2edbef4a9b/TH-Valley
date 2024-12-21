#include "map.h"

#include "cocos2d.h"
#include "frontend/bag_gui.h"
#include "game/animals.h"
#include "game/crops.h"
#include "game/pig.h"
#include "game/vegetables.h"
#include "ui/CocosGUI.h"

bool Map::initWithTMXFile(const std::string& tmxFile, cocos2d::Sprite* Haley,
                          ToolBar* CurrentToolBar, BagGUI* CurrentBag) {
    if (!cocos2d::TMXTiledMap::initWithTMXFile(tmxFile)) {
        return false;
    }

    // test animal
    for (int i = 0; i < 0; i++) {
        Pig* testPig;
        testPig = new Pig;
        MapAnimals.push_back(testPig);
    }

    tileMap = this;  // Assign the current instance to tileMap
    this->setAnchorPoint(cocos2d::Vec2(0, 0));
    this->setPosition(cocos2d::Vec2(0, 0));

    std::vector<std::string> layerNames = {"Back",     "Back2",    "Block",
                                           "Interact", "Building", "Paths",
                                           "Front",    "Front2"};

    int zOrder = -5;
    for (const auto& name : layerNames) {
        auto layer = tileMap->getLayer(name);
        if (layer) {
            if (name == "Front") {
                zOrder += 5;
            }
            layer->setLocalZOrder(zOrder++);
            mapLayer[name] = layer;
            CCLOG("Layer '%s' added with z-order %d", name.c_str(), zOrder - 1);
        } else {
            CCLOG("Layer '%s' not found", name.c_str());
        }
    }

    objectGroup = tileMap->getObjectGroup("Objects");
    if (objectGroup == nullptr) {
        CCLOG("ObjectGroup 'Objects' not found");
    } else {
        CCLOG("ObjectGroup 'Objects' loaded successfully");
    }

    auto player = objectGroup->getObject("Player");
    if (!player.empty()) {
        float x = player.at("x").asFloat();
        float y = player.at("y").asFloat();
        playerPos = cocos2d::Vec2(x, y);

    } else {
        CCLOG("Player object not found");
        playerPos = cocos2d::Vec2::ZERO;
    }
    CCLOG("Player position set to: %f, %f", playerPos.x, playerPos.y);
    CCLOG("Player position set to Tile: %f, %f", tileCoordFromPos(playerPos).x,
          tileCoordFromPos(playerPos).y);
    setViewpointCenter(playerPos);

    // auto texture =
    //     cocos2d::Director::getInstance()->getTextureCache()->addImage(
    //         "assets/Sebastian.png");

    // cocos2d::Rect frameRect(0, 0, 16, 32);

    // auto spriteFrame =
    //     cocos2d::SpriteFrame::createWithTexture(texture, frameRect);

    // playerSprite = cocos2d::Sprite::createWithSpriteFrame(spriteFrame);

    // ToolBar init
    MapToolBar = CurrentToolBar;
    MapBag = CurrentBag;
    // Player init
    playerSprite = Haley;
    playerSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.0f));
    playerSprite->setPosition(playerPos);

    // Animal init
    initAnimalPosition();

    CCLOG("Player sprite created at %f %f", playerPos.x, playerPos.y);
    CCLOG("Player sprite created at Tile: %f %f", tileCoordFromPos(playerPos).x,
          tileCoordFromPos(playerPos).y);

    this->addChild(playerSprite, 2);

    return true;
}

Map* Map::create(const std::string& tmxFile, cocos2d::Sprite* Haley,
                 ToolBar* CurrentToolBar, BagGUI* CurrentBag) {
    Map* ret = new (std::nothrow) Map();
    if (ret &&
        ret->initWithTMXFile(tmxFile, Haley, CurrentToolBar, CurrentBag)) {
        ret->autorelease();
        return ret;
    } else {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

void Map::setPlayerPos(cocos2d::Vec2 pos) {
    auto mapSize = this->getMapSize();
    auto tileSize = this->getTileSize();
    float mapWidth = mapSize.width * tileSize.width;
    float mapHeight = mapSize.height * tileSize.height;

    pos.x = std::max(0.0f, std::min(pos.x, mapWidth));
    pos.y = std::max(0.0f, std::min(pos.y, mapHeight));

    if (isCollisionAtAnyLayer(pos)) {
        CCLOG("Collision detected, player position not updated");
    } else {
        playerPos = pos;
        CCLOG("Player position set to: %f, %f", pos.x, pos.y);
        CCLOG("Tile: %f, %f", tileCoordFromPos(pos).x, tileCoordFromPos(pos).y);

        setViewpointCenter(pos);

        playerSprite->setPosition(playerPos);
    }
}

cocos2d::Vec2 Map::tileCoordFromPos(cocos2d::Vec2 pos) {
    // pos = this->convertToNodeSpace(pos);
    int x = pos.x / tileMap->getTileSize().width;
    int y =
        (tileMap->getMapSize().height * tileMap->getTileSize().height - pos.y) /
        tileMap->getTileSize().height;
    return cocos2d::Vec2(x, y);
}

cocos2d::Vec2 Map::PosFromtileCoord(Position PlantPosition) {
    cocos2d::Vec2 PicturePosition;
    PicturePosition.x = PlantPosition.x * tileMap->getTileSize().width;
    PicturePosition.y =
        (tileMap->getMapSize().height * tileMap->getTileSize().height) -
        PlantPosition.y * tileMap->getTileSize().height;
    PicturePosition.x += tileMap->getTileSize().width / 2;
    PicturePosition.y -= tileMap->getTileSize().height / 2;
    return PicturePosition;
}

void Map::setViewpointCenter(cocos2d::Vec2 pos) {
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

bool Map::isCollision(cocos2d::Vec2 pos, std::string LayerName) {
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

bool Map::isCollisionAtAnyLayer(cocos2d::Vec2 pos) {
    for (const auto& pair : mapLayer) {
        if (isCollision(pos, pair.first)) {
            return true;
        }
    }
    return false;
}

bool Map::isPortal(cocos2d::Vec2 pos, std::string ObjectLayerName) {
    auto objectGroup = this->getObjectGroup(ObjectLayerName);
    if (objectGroup == nullptr) {
        CCLOG("ObjectGroup %s not found", ObjectLayerName.c_str());
        return false;
    }
    auto objects = objectGroup->getObjects();
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
                std::string portalName = dict["name"].asString();
                CCLOG("Portal %s detected.", portalName.c_str());
                triggerPortalEvent(portalName);
                return true;
            }
        }
    }
    return false;
}

void Map::triggerPortalEvent(const std::string& portalName) {
    if (portalName == "FarmToHome") {
        CCLOG("Triggering %s event", portalName.c_str());
        auto house = Map::create("assets/maps/FarmHouse.tmx", playerSprite,
                                 MapToolBar, MapBag);
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

void Map::checkEventsAndTrigger(cocos2d::Vec2 tileCoord) {
    // int gid = collisionLayer->getTileGIDAt(tileCoord);
    // if (gid) {
    //     auto properties = this->getPropertiesForGID(gid).asValueMap();
    //     if (properties["Event"].asString() == "True") {
    //
    //         CCLOG("Event triggered at tile: %f, %f", tileCoord.x,
    //         tileCoord.y);
    //
    //     }
    // }
}

cocos2d::Vec2 Map::getPos() { return playerPos; }

void Map::updateTileAt(cocos2d::Vec2 tileCoord, int newGID,
                       std::string LayerName) {
    auto layer = tileMap->getLayer(LayerName);
    layer->setTileGID(newGID, tileCoord);
    CCLOG("Tile updated at %f, %f with GID: %d", tileCoord.x, tileCoord.y,
          newGID);
}

int Map::getTileID(cocos2d::Vec2 tileCoord, std::string LayerName) {
    auto layer = tileMap->getLayer(LayerName);
    int gid = layer->getTileGIDAt(tileCoord);
    if (gid == 0) {
        CCLOG("No tile at this position\n");
        return 0;
    }
    return gid;
}

bool Map::PropertyCheck(int gid, std::string property) {
    cocos2d::Value properties = tileMap->getPropertiesForGID(gid);
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

void Map::onEnter() {
    cocos2d::TMXTiledMap::onEnter();
    CCLOG("Map onEnter");

    auto listener = cocos2d::EventListenerMouse::create();

    listener->onMouseDown = [this](cocos2d::Event* event) {
        auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
        auto pos = this->convertToNodeSpace(mouseEvent->getLocationInView());
        auto tilePos = tileCoordFromPos(pos);

        CCLOG("Mouse Down Tile: %f, %f", tilePos.x, tilePos.y);
        CCLOG("Mouse Down: %f, %f", pos.x, pos.y);

        for (const auto& layer : mapLayer) {
            if (layer.second != nullptr) {
                int gid = layer.second->getTileGIDAt(tilePos);
                CCLOG("Checking layer: %s, gid: %d", layer.first.c_str(), gid);
                isCollision(pos, layer.first);
            }
        }

        // Plant
        int gid = getTileID(tilePos, "Back");
        if (PropertyCheck(gid, "Cultivable") &&
            MapToolBar->getToolName() == "Hoe") {
            if (MapToolBar != nullptr)
                MapToolBar->outputindex();
            else
                CCLOG("MapToolBar is nullptr");
            auto PlayerTilePos = tileCoordFromPos(playerPos);
            if (fabs(PlayerTilePos.x - tilePos.x) > 1 ||
                fabs(PlayerTilePos.y - tilePos.y) > 1) {
                CCLOG("Too far to cultivate");
            } else {
                updateTileAt(tilePos, 557, "Back");
                CCLOG("Tile is cultivable\n");
            }
        } else {
            Position PlantTilePos;
            PlantTilePos.x = tilePos.x;
            PlantTilePos.y = tilePos.y;

            // take a Strawberry as an example
            /*Strawberry* exampleStrawberry;
            exampleStrawberry = new Strawberry;*/

            Potato* exampleStrawberry;
            exampleStrawberry = new Potato;
            CCLOG("Potato");

            CropPlant(PlantTilePos, exampleStrawberry);
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
                isKeyPressedW = true;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                isKeyPressedS = true;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                isKeyPressedA = true;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                isKeyPressedD = true;
                break;
            default:
                break;
        }
    };

    keyListener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode,
                                        cocos2d::Event* event) {
        switch (keyCode) {
            case cocos2d::EventKeyboard::KeyCode::KEY_W:
                isKeyPressedW = false;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                isKeyPressedS = false;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                isKeyPressedA = false;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                isKeyPressedD = false;
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

void Map::createMiniMap() {
    auto miniMap = cocos2d::Sprite::create();
    miniMap->setScale(0.1f);
    miniMap->setPosition(cocos2d::Vec2(500, 500));
    auto playerMarker = cocos2d::Sprite::create();
    playerMarker->setColor(cocos2d::Color3B::RED);
    playerMarker->setScale(0.2f);
    this->addChild(miniMap, 100);
}

void Map::save() {}

void Map::load() {}

void Map::update(float delta) {
    cocos2d::Vec2 currentPos = getPos();
    float moveStep = 80.0f * delta;

    if (isKeyPressedW) {
        currentPos.y += moveStep;
    }
    if (isKeyPressedS) {
        currentPos.y -= moveStep;
    }
    if (isKeyPressedA) {
        currentPos.x -= moveStep;
    }
    if (isKeyPressedD) {
        currentPos.x += moveStep;
    }

    if (isKeyPressedW || isKeyPressedS || isKeyPressedA || isKeyPressedD) {
        setPlayerPos(currentPos);
    }
}
