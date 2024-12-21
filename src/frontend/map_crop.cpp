#include "cocos2d.h"
#include "game/crops.h"
#include "map.h"
#include "ui/CocosGUI.h"

void Map::CropPlant(const Position& PlantPosition, Crops* Crop) {
    // Position check
    // static int priority = 3;
    auto PlayerTilePos = tileCoordFromPos(playerPos);
    if (fabs(PlayerTilePos.x - PlantPosition.x) > 1 ||
        fabs(PlayerTilePos.y - PlantPosition.y) > 1) {
        CCLOG("Too far to plant");
        return;
    }

    // Planted Check
    if (CropPosition[PlantPosition]) {
        CCLOG("This tile has been planted");
        ShowCropInformation(CropPosition[PlantPosition], PlantPosition,
                            priority);
        return;
    }

    // Soil Check
    int SoilCheck = 0;
    int gid =
        getTileID(cocos2d::Vec2(PlantPosition.x, PlantPosition.y), "Back");
    for (int i = 0; i < Crop->SoilRequirement.size(); i++)
        if (PropertyCheck(gid, Crop->SoilRequirement[i])) {
            SoilCheck = 1;
            break;
        }
    if (!SoilCheck) {
        CCLOG("The Soil is not proper\n");
        return;
    }

    CCLOG("Can be planted");

    auto CropPicture = Crop->CropSprite;

    // Picture Position Calculate
    cocos2d::Vec2 PicturePosition;
    PicturePosition = PosFromtileCoord(PlantPosition);

    CCLOG("Size: %f %f\n", tileMap->getTileSize().width,
          tileMap->getTileSize().height);
    CCLOG("PicturePosition: %f %f\n", PicturePosition.x, PicturePosition.y);
    CCLOG("PlantPosition: %f %f\n", PlantPosition.x, PlantPosition.y);
    CropPicture->setPosition(PicturePosition);

    CropPosition[PlantPosition] = Crop;
    SpritePosition[PlantPosition] = CropPicture;
    MapCrops.push_back(Crop);

    this->addChild(CropPicture, 1);
}

void Map::CropRemove(const Position& RemovePosition) {
    Crops* Crop = CropPosition[RemovePosition];
    auto Picture = SpritePosition[RemovePosition];
    std::vector<Crops*>::iterator it;

    // Remove from vector
    for (it = MapCrops.begin(); it != MapCrops.end(); it++) {
        if (*it == Crop) {
            MapCrops.erase(it);
            break;
        }
    }

    // Remove Picture
    Picture->setVisible(false);

    // Remove pointer
    delete Crop;

    // Remove from map
    CropPosition[RemovePosition] = 0;
    SpritePosition[RemovePosition] = 0;
}

void Map::CropUpdate(const Position& UpdatePosition) {
    auto Picture = SpritePosition[UpdatePosition];
    Crops* Crop = CropPosition[UpdatePosition];
    auto newPicture =
        cocos2d::Sprite::create("assets/TileSheets/crops.png",
                                Crop->frameRect[Crop->CurrentGrowthStage]);
    Picture = newPicture;
    delete newPicture;
}

void Map::ShowCropInformation(Crops* Crop, const Position& InfoPosition,
                              int& priority) {
    // Turn Tile position to scene position
    cocos2d::Vec2 PicturePosition = PosFromtileCoord(InfoPosition);

    // infoBox Define
    auto infoBox =
        cocos2d::Sprite::create("assets/testpng/boardgameborder.png");
    infoBox->setPosition(
        cocos2d::Vec2(PicturePosition.x, PicturePosition.y + 40));
    infoBox->setScale(0.64);
    this->addChild(infoBox, priority);

    // Title Define
    size_t kFontSize = 50;
    auto* InfoTitle = cocos2d::Label::createWithTTF(
        Crop->CropName, "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
    InfoTitle->setScale(0.1);
    InfoTitle->setPosition(
        cocos2d::Vec2(PicturePosition.x - 20, PicturePosition.y + 50));
    this->addChild(InfoTitle, ++priority);

    // Button Define
    auto HavestButton = cocos2d::ui::Button::create(
        "assets/testpng/button.png", "assets/testpng/buttonpress.png");
    auto WaterButton = cocos2d::ui::Button::create(
        "assets/testpng/button.png", "assets/testpng/buttonpress.png");
    auto FertilizeButton = cocos2d::ui::Button::create(
        "assets/testpng/button.png", "assets/testpng/buttonpress.png");
    auto CloseButton = cocos2d::ui::Button::create(
        "assets/testpng/button.png", "assets/testpng/buttonpress.png");

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
         InfoTitle, Stage, DeathInfo, Situation, Crop, InfoPosition,
         Title](cocos2d::Ref* sender) {
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
            if (Title == "Remove") CropRemove(InfoPosition);
            // Here do harvest
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
        });
    this->addChild(CloseButton, priority++);
}
