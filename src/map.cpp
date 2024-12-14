#include "map.h"

bool Map::initWithTMXFile(const std::string& tmxFile) {
    // Change the default resource root path from "Resources/" to "assets/".
    SetResourcePath("assets");

    if (!cocos2d::TMXTiledMap::initWithTMXFile(tmxFile)) {
        return false;
    }

    tileMap = this;  // Assign the current instance to tileMap

    // 初始化地图的其他内容
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
    // 判断是否发生碰撞
    if (isCollision(pos, "Paths") && isCollision(pos, "Block") &&
        isCollision(pos, "Building") && isCollision(pos, "Interact")) {
        CCLOG("Collision detected, player position not updated");
    } else {
        // 更新玩家位置
        // 假设有一个player对象
        // player->setPosition(pos);
        CCLOG("Player position set to: %f, %f", pos.x, pos.y);
    }
    
}

cocos2d::Vec2 Map::tileCoordFromPos(cocos2d::Vec2 pos) {
    // 将像素坐标转换为瓦片坐标
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

    // 屏幕中心点
    cocos2d::Vec2 centerPoint =
        cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2);
    // 使精灵处于屏幕中心，移动地图目标位置
    cocos2d::Vec2 actualPoint = cocos2d::Vec2(x, y);
    CCLOG("actualPoint: %f, %f", actualPoint.x, actualPoint.y);

    // 地图移动偏移量
    cocos2d::Vec2 offset = centerPoint - actualPoint;

    CCLOG("offset: %f, %f", offset.x, offset.y);

    this->setPosition(offset);
}

bool Map::isCollision(cocos2d::Vec2 pos, std::string LayerName) {
    auto Layer = this->getLayer(LayerName);

    // 获取指定位置的瓦片坐标
    auto tileCoord = this->tileCoordFromPos(pos);

    // 检查该位置是否有瓦片
    int tileGid = Layer->getTileGIDAt(tileCoord);
    if (tileGid != 0) {
        // 获取瓦片的属性
        auto properties = this->getPropertiesForGID(tileGid);

        // 确保属性类型为 MAP，避免断言失败
        if (properties.getType() == cocos2d::Value::Type::MAP) {
            auto valueMap = properties.asValueMap();

            // 检查 "Collidable" 属性是否存在且为 "true"
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
                // 根据具体名称完成不同的切图操作
                std::string portalName = dict["name"].asString();
                CCLOG("Portal %s detected.", portalName.c_str());
                //if (portalName == "PortalA") {
                //    // 执行 PortalA 的切图操作
                //    CCLOG("Triggering PortalA event");
                //    // this->triggerPortalAEvent();
                //} else if (portalName == "PortalB") {
                //    // 执行 PortalB 的切图操作
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
    //// 获取瓦片坐标对应的gid
    //int gid = collisionLayer->getTileGIDAt(tileCoord);
    //if (gid) {
    //    auto properties = this->getPropertiesForGID(gid).asValueMap();
    //    if (properties["Event"].asString() == "True") {
    //        // 触发事件
    //        CCLOG("Event triggered at tile: %f, %f", tileCoord.x, tileCoord.y);
    //        // 根据事件类型执行相应的逻辑
    //        // 例如：this->triggerBattleEvent();
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
    //// 创建一个小地图的精灵
    //auto miniMap = cocos2d::Sprite::create();
    //miniMap->setScale(0.1f);                        // 缩小比例
    //miniMap->setPosition(cocos2d::Vec2(100, 100));  // 设置小地图的位置

    //// 添加小地图到当前地图层
    //this->addChild(miniMap, 100);  // 100 是 z-order，确保小地图
}

void Map::save() {
    // 获取玩家位置
    

    // 获取地图偏移
    

    // 保存状态到文件
    
}

void Map::load() {
    // 从文件中读取状态

    // 设置玩家位置

    // 设置地图偏移
}
