#include "map.h"

bool Map::initWithTMXFile(const std::string& tmxFile) {
    // Change the default resource root path from "Resources/" to "assets/".
    SetResourcePath("assets");

    if (!cocos2d::TMXTiledMap::initWithTMXFile(tmxFile)) {
        return false;
    }

    tileMap = this;  // Assign the current instance to tileMap

    // ��ʼ����ͼ����������
    mapLayer = {tileMap->getLayer("Back"),     tileMap->getLayer("Back2"),
                tileMap->getLayer("Block"),    tileMap->getLayer("Interact"),
                tileMap->getLayer("Building"), tileMap->getLayer("Paths"),
                tileMap->getLayer("Front")};
    objectGroup = tileMap->getObjectGroup("Objects");

    for (const auto& layer : mapLayer) {
        if (layer == nullptr) {
            CCLOG("Map layer '%s' not found",
                  layer->getLayerName().c_str());
        } else {
            CCLOG("Map layer '%s' loaded successfully",
                  layer->getLayerName().c_str());
        }
    }

    if (objectGroup == nullptr) {
        CCLOG("ObjectGroup 'Objects' not found");
    } else {
        CCLOG("ObjectGroup 'Objects' loaded successfully");
    }
    return true;
}

Map* Map::create(const std::string& tmxFile) {
    Map* ret = new (std::nothrow) Map();
    if (ret && ret->initWithTMXFile(tmxFile)) {
        ret->autorelease();
        return ret;
    } else {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

void Map::setPlayerPos(cocos2d::Vec2 pos) {
    // �ж��Ƿ�����ײ
    if (isCollision(pos, "Paths") && isCollision(pos, "Block") &&
        isCollision(pos, "Building") && isCollision(pos, "Interact")) {
        CCLOG("Collision detected, player position not updated");
    } else {
        // �������λ��
        // ������һ��player����
        // player->setPosition(pos);
        CCLOG("Player position set to: %f, %f", pos.x, pos.y);
    }
    
}

cocos2d::Vec2 Map::tileCoordFromPos(cocos2d::Vec2 pos) {
    // ����������ת��Ϊ��Ƭ����
    int x = pos.x / tileMap->getTileSize().width;
    int y =
        (tileMap->getMapSize().height * tileMap->getTileSize().height - pos.y) /
        tileMap->getTileSize().height;
    return cocos2d::Vec2(x, y);
}

void Map::setViewpointCenter(cocos2d::Vec2 pos) {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    int x = MAX(pos.x, visibleSize.width / 2);
    int y = MAX(pos.y, visibleSize.height / 2);
    x = MIN(x, (tileMap->getMapSize().width * tileMap->getTileSize().width) -
                   visibleSize.width / 2);
    y = MIN(y, (tileMap->getMapSize().height * tileMap->getTileSize().height) -
                   visibleSize.height / 2);

    // ��Ļ���ĵ�
    cocos2d::Vec2 centerPoint =
        cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2);
    // ʹ���鴦����Ļ���ģ��ƶ���ͼĿ��λ��
    cocos2d::Vec2 actualPoint = cocos2d::Vec2(x, y);
    CCLOG("actualPoint: %f, %f", actualPoint.x, actualPoint.y);

    // ��ͼ�ƶ�ƫ����
    cocos2d::Vec2 offset = centerPoint - actualPoint;

    CCLOG("offset: %f, %f", offset.x, offset.y);

    this->setPosition(offset);
}

bool Map::isCollision(cocos2d::Vec2 pos, std::string LayerName) {
    auto Layer = this->getLayer(LayerName);

    // ��ȡָ��λ�õ���Ƭ����
    auto tileCoord = this->tileCoordFromPos(pos);

    // ����λ���Ƿ�����Ƭ
    int tileGid = Layer->getTileGIDAt(tileCoord);
    if (tileGid != 0) {
        // ��ȡ��Ƭ������
        auto properties = this->getPropertiesForGID(tileGid);

        // ȷ����������Ϊ MAP���������ʧ��
        if (properties.getType() == cocos2d::Value::Type::MAP) {
            auto valueMap = properties.asValueMap();

            // ��� "Collidable" �����Ƿ������Ϊ "true"
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
            if (rect.containsPoint(pos)) {
                CCLOG("Portal detected at tile: %f, %f", pos.x, pos.y);
                // ���ݾ���������ɲ�ͬ����ͼ����
                std::string portalName = dict["name"].asString();
                CCLOG("Portal %s detected.", portalName.c_str());
                //if (portalName == "PortalA") {
                //    // ִ�� PortalA ����ͼ����
                //    CCLOG("Triggering PortalA event");
                //    // this->triggerPortalAEvent();
                //} else if (portalName == "PortalB") {
                //    // ִ�� PortalB ����ͼ����
                //    CCLOG("Triggering PortalB event");
                //    // this->triggerPortalBEvent();
                //} else {
                //    CCLOG("Unknown portal: %s", portalName.c_str());
                //}
                return true;
            }
        }
    }
    return false;
}

void Map::checkEventsAndTrigger(cocos2d::Vec2 tileCoord) {
    //// ��ȡ��Ƭ�����Ӧ��gid
    //int gid = collisionLayer->getTileGIDAt(tileCoord);
    //if (gid) {
    //    auto properties = this->getPropertiesForGID(gid).asValueMap();
    //    if (properties["Event"].asString() == "True") {
    //        // �����¼�
    //        CCLOG("Event triggered at tile: %f, %f", tileCoord.x, tileCoord.y);
    //        // �����¼�����ִ����Ӧ���߼�
    //        // ���磺this->triggerBattleEvent();
    //    }
    //}
}

cocos2d::Vec2 Map::getStartPos() {
    auto object = objectGroup->getObject("SpawnPoint");
    float x = object.at("x").asFloat();
    float y = object.at("y").asFloat();
    return cocos2d::Vec2(x, y);
}

cocos2d::Vec2 Map::getPos() {
    auto object = objectGroup->getObject("Player");
    if (object.empty()) {
        CCLOG("Player object not found");
        return cocos2d::Vec2::ZERO;
    }
    float x = object.at("x").asFloat();
    float y = object.at("y").asFloat();
    return cocos2d::Vec2(x, y);
}

void Map::onEnter() {
    cocos2d::TMXTiledMap::onEnter();
    CCLOG("Map onEnter");

    auto listener = cocos2d::EventListenerMouse::create();

    listener->onMouseDown = [this](cocos2d::Event* event) {
        auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
        auto pos = mouseEvent->getLocationInView();
        auto tilePos = tileCoordFromPos(pos);
        CCLOG("Mouse Down Tile: %f, %f", tilePos.x, tilePos.y);
        CCLOG("Mouse Down: %f, %f", pos.x, pos.y);

        for (const auto& layer : mapLayer) {
            if (layer != nullptr) {
                CCLOG("Checking layer: %s", layer->getLayerName().c_str());
                isCollision(pos, layer->getLayerName().c_str());
            }
        }
        isPortal(pos);
    };

    listener->onMouseMove = [this](cocos2d::Event* event) {
        auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
        auto pos = mouseEvent->getLocationInView();
        auto tilePos = tileCoordFromPos(pos);
        //CCLOG("Mouse Move Tile: %f, %f", tilePos.x, tilePos.y);
        //CCLOG("Mouse Move: %f, %f", pos.x, pos.y);
    };

    cocos2d::EventDispatcher* dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Map::SetResourcePath(const std::string& path) {
    auto* file_utils = cocos2d::FileUtils::getInstance();

    // Get the current resource root absolute path.
    std::string resource_root_path =
        cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();

    // Replace the last directory name with the new path.
    const size_t pos =
        resource_root_path.find_last_of('/', resource_root_path.size() - 2);
    if (pos != std::string::npos) {
        resource_root_path.replace(pos + 1, std::string::npos, path + "/");
    } else {
        resource_root_path = path + "/";
    }

    // Update the default resource root path.
    file_utils->setDefaultResourceRootPath(resource_root_path);
}

void Map::createMiniMap() {
    //// ����һ��С��ͼ�ľ���
    //auto miniMap = cocos2d::Sprite::create();
    //miniMap->setScale(0.1f);                        // ��С����
    //miniMap->setPosition(cocos2d::Vec2(100, 100));  // ����С��ͼ��λ��

    //// ���С��ͼ����ǰ��ͼ��
    //this->addChild(miniMap, 100);  // 100 �� z-order��ȷ��С��ͼ
}

void Map::save() {
    // ��ȡ���λ��
    

    // ��ȡ��ͼƫ��
    

    // ����״̬���ļ�
    
}

void Map::load() {
    // ���ļ��ж�ȡ״̬

    // �������λ��

    // ���õ�ͼƫ��
}
