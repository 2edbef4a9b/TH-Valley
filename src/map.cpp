#include "map.h"
#include "Crops.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

bool Map::initWithTMXFile(const std::string& tmxFile) {
    

    if (!cocos2d::TMXTiledMap::initWithTMXFile(tmxFile)) {
        return false;
    }

    tileMap = this;  // Assign the current instance to tileMap
    this->setAnchorPoint(cocos2d::Vec2(0, 0));
    this->setPosition(cocos2d::Vec2(0, 0));

    // ��ʼ����ͼ����������
    std::vector<std::string> layerNames = {
        "Back", "Back2", "Block", "Interact", "Building", "Paths", "Front", "Front2"};

    int zOrder = -5;  // ��ʼ z-order���� 0 ��ʼ
    for (const auto& name : layerNames) {
        auto layer = tileMap->getLayer(name);
        if (layer) {
            if (name == "Front") {
                zOrder += 5;
            }
            layer->setLocalZOrder(zOrder++);  // ÿ����� z-order
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

    // ��ȡ��ʼ���λ��
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
    setViewpointCenter(playerPos);  // �����ӽ�����

    // ��������
    auto texture =
        cocos2d::Director::getInstance()->getTextureCache()->addImage(
            "assets/Sebastian.png");
    // ����ü����򣨵�һ��ͼƬ��λ�úͳߴ磩
    cocos2d::Rect frameRect(0, 0, 16, 32);
    // ��������֡
    auto spriteFrame =
        cocos2d::SpriteFrame::createWithTexture(texture, frameRect);
    // �������鲢���þ���֡
    playerSprite = cocos2d::Sprite::createWithSpriteFrame(spriteFrame);
    // ����ê��Ϊ�ײ�����
    playerSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.0f));
    // ���þ���λ��
    playerSprite->setPosition(playerPos);
    CCLOG("Player sprite created at %f %f", playerPos.x, playerPos.y);
    CCLOG("Player sprite created at Tile: %f %f", tileCoordFromPos(playerPos).x,
          tileCoordFromPos(playerPos).y);
    // ��������ӵ���ͼ
    this->addChild(playerSprite, 2);

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
    auto mapSize = this->getMapSize();
    auto tileSize = this->getTileSize();
    float mapWidth = mapSize.width * tileSize.width;
    float mapHeight = mapSize.height * tileSize.height;

    // �߽��飬ȷ������λ���ڵ�ͼ��Χ��
    pos.x = std::max(0.0f, std::min(pos.x, mapWidth));
    pos.y = std::max(0.0f, std::min(pos.y, mapHeight));

    if (isCollisionAtAnyLayer(pos)) {
        CCLOG("Collision detected, player position not updated");
    } else {
        playerPos = pos;
        CCLOG("Player position set to: %f, %f", pos.x, pos.y);
        CCLOG("Tile: %f, %f", tileCoordFromPos(pos).x, tileCoordFromPos(pos).y);

        setViewpointCenter(pos);  // �����ӽ�����

        // ������Ҿ����λ��
        playerSprite->setPosition(playerPos);
    }
}

cocos2d::Vec2 Map::tileCoordFromPos(cocos2d::Vec2 pos) {
    //pos = this->convertToNodeSpace(pos);
    // ����������ת��Ϊ��Ƭ����
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
        (tileMap->getMapSize().height * tileMap->getTileSize().height -
         PlantPosition.y) -
        PlantPosition.y * tileMap->getTileSize().height;
    PicturePosition.x += tileMap->getTileSize().width / 2;
    PicturePosition.y += tileMap->getTileSize().height / 2;
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

    // ��Ļ���ĵ�
    cocos2d::Vec2 centerPoint =
        cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2);
    // ʹ���鴦����Ļ���ģ��ƶ���ͼĿ��λ��
    cocos2d::Vec2 actualPoint = cocos2d::Vec2(x, y);
    CCLOG("actualPoint: %f, %f", actualPoint.x, actualPoint.y);

    // ��ͼ�ƶ�ƫ����
    cocos2d::Vec2 offset = centerPoint - actualPoint;

    CCLOG("offset: %f, %f", offset.x, offset.y);

    // �����ͼС�ڴ��ڴ�С���������ʾ
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


            //auto it2 = valueMap.find("Cultivable");
            //if (it2 != valueMap.end() && it2->second.asString() == "true") {
            //    CCLOG("LAND detected at tile: %f, %f at Layer %s",
            //          tileCoord.x, tileCoord.y, LayerName.c_str());
            //    updateTileAt(tileCoord, 557, LayerName);
            //}

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
                // ���ݾ���������ɲ�ͬ����ͼ����
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
        auto house = Map::create("assets/maps/FarmHouse.tmx");
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

cocos2d::Vec2 Map::getPos() { return playerPos; }

void Map::updateTileAt(cocos2d::Vec2 tileCoord, int newGID, std::string LayerName) {
    auto layer = tileMap->getLayer(LayerName);
    layer->setTileGID(newGID, tileCoord);
    CCLOG("Tile updated at %f, %f with GID: %d", tileCoord.x, tileCoord.y, newGID);
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
                CCLOG("Checking layer: %s, gid: %d",
                      layer.first.c_str(), gid);
                isCollision(pos, layer.first);
            }
        }

        // Plant
        // if (Seed in Hand): 
        Position PlantTilePos;
        PlantTilePos.x = tilePos.x;
        PlantTilePos.y = tilePos.y;

        // take a Strawberry as an example
        Strawberry *exampleStrawberry;
        exampleStrawberry = new Strawberry;

        CropPlant(PlantTilePos, exampleStrawberry);

        isPortal(pos);
    };

    listener->onMouseMove = [this](cocos2d::Event* event) {
        auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
        auto pos = this->convertToNodeSpace(mouseEvent->getLocationInView());
        auto tilePos = tileCoordFromPos(pos);
        //CCLOG("Mouse Move Tile: %f, %f", tilePos.x, tilePos.y);
        //CCLOG("Mouse Move: %f, %f", pos.x, pos.y);
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


    cocos2d::EventDispatcher* dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    this->scheduleUpdate();
}

void Map::createMiniMap() {
    // ����һ��С��ͼ�ľ���
    auto miniMap = cocos2d::Sprite::create();
    miniMap->setScale(0.1f);                        // ��С����
    miniMap->setPosition(cocos2d::Vec2(500, 500));  // ����С��ͼ��λ��
    // ������ұ��
    auto playerMarker = cocos2d::Sprite::create();
    playerMarker->setColor(cocos2d::Color3B::RED);
    playerMarker->setScale(0.2f);
    // ���С��ͼ����ǰ��ͼ��
    this->addChild(miniMap, 100);  // 100 �� z-order��ȷ��С��ͼ
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



void Map::update(float delta) {
    cocos2d::Vec2 currentPos = getPos();
    float moveStep = 100.0f * delta;  // ȷ���ƶ��ٶ���֡���޹�

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

void Map::CropPlant(const Position& PlantPosition, Crops* Crop) {
    // Position check
    static int priority = 3;
    auto PlayerTilePos = tileCoordFromPos(playerPos);
    if (fabs(PlayerTilePos.x - PlantPosition.x) > 1 ||
        fabs(PlayerTilePos.y - PlantPosition.y) > 1) {
        CCLOG("Too far to plant");
        return;
    }

    // Planted Check
    if (CropPosition[PlantPosition]) {
        CCLOG("This tile has been planted");
        ShowCropInformation(CropPosition[PlantPosition], PlantPosition, priority);
        return;
    }

    CCLOG("Can be planted");

    auto CropPicture = cocos2d::Sprite::create("assets/TileSheets/crops.png",
                                               cocos2d::Rect(1, 593, 13, 14));

    // Picture Position Calculate
    cocos2d::Vec2 PicturePosition;
    PicturePosition = PosFromtileCoord(PlantPosition);

    CCLOG("Size: %f %f\n", tileMap->getTileSize().width,
          tileMap->getTileSize().height);
    CCLOG("PicturePosition: %f %f\n", PicturePosition.x, PicturePosition.y);
    CCLOG("PlantPosition: %f %f\n", PlantPosition.x, PlantPosition.y);
    CropPicture->setPosition(PicturePosition);

    CropPosition[PlantPosition] = Crop;
    MapCrops.push_back(Crop);

    this->addChild(CropPicture, 1);
}

void Map::ShowCropInformation(Crops* Crop, const Position& InfoPosition, int& priority) {
    // Turn Tile position to scene position
    cocos2d::Vec2 PicturePosition = PosFromtileCoord(InfoPosition);

    // infoBox Define
    auto infoBox = cocos2d::Sprite::create("assets/testpng/boardgameborder.png");
    infoBox->setPosition(cocos2d::Vec2(PicturePosition.x, PicturePosition.y + 40));
    infoBox->setScale(0.64);
    this->addChild(infoBox, priority);

    // Title Define
    size_t kFontSize = 50;
    auto* InfoTitle = cocos2d::Label::createWithTTF(
        Crop->CropName, "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
    InfoTitle->setScale(0.1);
    InfoTitle->setPosition(cocos2d::Vec2(PicturePosition.x - 20, PicturePosition.y + 50));
    this->addChild(InfoTitle, ++priority);

    
    // Button Define
    auto HavestButton = cocos2d::ui::Button::create("assets/testpng/button.png",
                                                    "assets/testpng/buttonpress.png");
    auto WaterButton = cocos2d::ui::Button::create("assets/testpng/button.png",
                                                   "assets/testpng/buttonpress.png");
    auto FertilizeButton = cocos2d::ui::Button::create(
        "assets/testpng/button.png", "assets/testpng/buttonpress.png");
    auto CloseButton = cocos2d::ui::Button::create("assets/testpng/button.png",
                                                   "assets/testpng/buttonpress.png");

    // Stage Define
    kFontSize = 50;
    std::string output;
    if (Crop->GrowthSituation[0] != "Death")
        output = "Stage: " + Crop->GrowthStage[Crop->CurrentGrowthStage];
    auto* Stage = cocos2d::Label::createWithTTF(
        output, "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
    Stage->setScale(0.1);
    Stage->setPosition(
        cocos2d::Vec2(PicturePosition.x + 20, PicturePosition.y + 50));
    Stage->setTextColor(cocos2d::Color4B::GRAY);
    this->addChild(Stage, priority);
    
    // DeathInfo Define
    kFontSize = 50;
    auto* DeathInfo = cocos2d::Label::createWithTTF(
        "Death", "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
    DeathInfo->setScale(0.1);
    DeathInfo->setPosition(
        cocos2d::Vec2(PicturePosition.x + 20, PicturePosition.y + 40));
    DeathInfo->setTextColor(cocos2d::Color4B::RED);
    this->addChild(DeathInfo, priority);

    // Situation Define
    kFontSize = 50;
    output = Crop->GrowthSituation[0] + "\n";
    for (int i = 1; i < Crop->GrowthSituation.size(); i++) {
        output = output + Crop->GrowthSituation[i] + "\n";
    }
    auto* Situation = cocos2d::Label::createWithTTF(
        output, "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
    Situation->setScale(0.1);
    Situation->setPosition(
        cocos2d::Vec2(PicturePosition.x + 20, PicturePosition.y + 35));
    Situation->setTextColor(cocos2d::Color4B::ORANGE);
    this->addChild(Situation, priority);

    // Print Special Situation
    if (Crop->GrowthSituation[0] != "Death") {
        DeathInfo->setVisible(false);
    }
    if (Crop->GrowthSituation[0] == "Normal" ||
        Crop->GrowthSituation[0] == "Death") {
            Situation->setVisible(false);
    }

    // HavestButton
    HavestButton->setScale(0.15);
    HavestButton->setPosition(
        cocos2d::Vec2(PicturePosition.x - 20, PicturePosition.y + 43));
    std::string Title;
    if (Crop->CurrentGrowthStage == Crop->MaxGrowthStage)
        Title = "Harvest";
    else
        Title = "Remove";
    HavestButton->setTitleText(Title);
    HavestButton->setTitleFontName("assets/fonts/DFHannotateW5-A.ttf");
    HavestButton->setTitleColor(cocos2d::Color3B::BLACK);
    HavestButton->setColor(cocos2d::Color3B::WHITE);
    HavestButton->setTitleFontSize(40);
    HavestButton->addClickEventListener(
        [this, HavestButton, WaterButton, FertilizeButton, CloseButton, infoBox,
         InfoTitle, Stage, DeathInfo, Situation, Crop](cocos2d::Ref* sender) {
            HavestButton->setVisible(false);
            WaterButton->setVisible(false);
            FertilizeButton->setVisible(false);
            infoBox->setVisible(false);
            InfoTitle->setVisible(false);
            Stage->setVisible(false);
            CloseButton->setVisible(false);
            if (Crop->GrowthSituation[0] == "Death") {
                DeathInfo->setVisible(false);
            }
            if (Crop->GrowthSituation[0] != "Normal") {
                Situation->setVisible(false);
            }
            // Here Do Harvest
        });
    this->addChild(HavestButton, priority);

    // WaterButton
    WaterButton->setScale(0.15);
    WaterButton->setPosition(
        cocos2d::Vec2(PicturePosition.x - 20, PicturePosition.y + 35));
    WaterButton->setTitleText("Water");
    WaterButton->setTitleFontName("assets/fonts/DFHannotateW5-A.ttf");
    WaterButton->setTitleColor(cocos2d::Color3B::BLACK);
    WaterButton->setColor(cocos2d::Color3B::WHITE);
    WaterButton->setTitleFontSize(40);
    WaterButton->addClickEventListener(
        [this, WaterButton, infoBox, InfoTitle, Crop](cocos2d::Ref* sender) {
            Crop->CropWatering();
        });
    this->addChild(WaterButton, priority);
    if (Crop->GrowthSituation[0] == "Death") WaterButton->setVisible(false);

    // FertilizeButton
    FertilizeButton->setScale(0.15);
    FertilizeButton->setPosition(
        cocos2d::Vec2(PicturePosition.x - 20, PicturePosition.y + 27));
    FertilizeButton->setTitleText("Fertilize");
    FertilizeButton->setTitleFontName("assets/fonts/DFHannotateW5-A.ttf");
    FertilizeButton->setTitleColor(cocos2d::Color3B::BLACK);
    FertilizeButton->setColor(cocos2d::Color3B::WHITE);
    FertilizeButton->setTitleFontSize(37);
    FertilizeButton->addClickEventListener(
        [this, infoBox, InfoTitle, Crop](cocos2d::Ref* sender) {
            Crop->CropFertilize();
        });
    this->addChild(FertilizeButton, priority);
    if (Crop->GrowthSituation[0] == "Death") FertilizeButton->setVisible(false);

    // Close Button
    CloseButton->setScale(0.15);
    CloseButton->setPosition(
        cocos2d::Vec2(PicturePosition.x + 20, PicturePosition.y + 24));
    CloseButton->setTitleText("Confirm");
    CloseButton->setTitleFontName("assets/fonts/DFHannotateW5-A.ttf");
    CloseButton->setTitleColor(cocos2d::Color3B::BLACK);
    CloseButton->setColor(cocos2d::Color3B::WHITE);
    CloseButton->setTitleFontSize(40);
    // When Click the "Confirm", infoBox and the information in it turn off
    CloseButton->addClickEventListener(
        [this, HavestButton, WaterButton, FertilizeButton, CloseButton, infoBox,
         InfoTitle, Stage, DeathInfo, Situation, Crop](cocos2d::Ref* sender) {
            HavestButton->setVisible(false);
            WaterButton->setVisible(false);
            FertilizeButton->setVisible(false);
            infoBox->setVisible(false);
            InfoTitle->setVisible(false);
            Stage->setVisible(false);
            CloseButton->setVisible(false);
            if (Crop->GrowthSituation[0] == "Death") {
                DeathInfo->setVisible(false);
            }
            if (Crop->GrowthSituation[0] != "Normal") {
                Situation->setVisible(false);
            }
            // Here Do Harvest
        });
    this->addChild(CloseButton, priority++);
}
