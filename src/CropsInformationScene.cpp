#include "CropsInformationScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Crops.h"

void CropsInformationScene::SetResourcePath(const std::string &path) {
    auto *file_utils = cocos2d::FileUtils::getInstance();

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

bool CropsInformationScene::init() {
    SetResourcePath("assets");

    /*test
    Strawberry* strawberry1;
    strawberry1 = new Strawberry;
    GetCropInformation(strawberry1);*/

    if (!cocos2d::Scene::init()) {
        return false;
    }

    // infoBox Define
    auto infoBox = cocos2d::Sprite::create("testpng/boardgameborder.png");
    infoBox->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2));
    infoBox->setScale(5.0);
    this->addChild(infoBox);

    size_t kFontSize = 40;
    auto *InfoTitle = cocos2d::Label::createWithTTF(
        Crop->CropName, "fonts/DFHannotateW5-A.ttf", kFontSize);
    InfoTitle->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2 - 200,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2 + 100));
    this->addChild(InfoTitle);

    // Button Define
    auto HavestButton = cocos2d::ui::Button::create("testpng/button.png",
                                                  "testpng/buttonpress.png");
    auto WaterButton = cocos2d::ui::Button::create("testpng/button.png",
                                                   "testpng/buttonpress.png");
    auto FertilizeButton = cocos2d::ui::Button::create("testpng/button.png",
                                                   "testpng/buttonpress.png");
    auto CloseButton = cocos2d::ui::Button::create("testpng/button.png",
                                                   "testpng/buttonpress.png");

    // Label Define
    // Name Define
    kFontSize = 40;
    std::string output;
    if (Crop->GrowthSituation[0] != "Death")
        output = "Stage: " + Crop->GrowthStage[Crop->CurrentGrowthStage];
    auto *Name = cocos2d::Label::createWithTTF(
        output, "fonts/DFHannotateW5-A.ttf", kFontSize);
    Name->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2 + 150,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2 + 80));
    Name->setTextColor(cocos2d::Color4B::GRAY);

    // DeathInfo Define
    kFontSize = 40;
    auto *DeathInfo = cocos2d::Label::createWithTTF(
        "Death", "fonts/DFHannotateW5-A.ttf", kFontSize);
    DeathInfo->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2 + 150,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2 - 40));
    DeathInfo->setTextColor(cocos2d::Color4B::RED);

    // Situation Define
    kFontSize = 40;
    output = Crop->GrowthSituation[0] + "\n";
    for (int i = 1; i < Crop->GrowthSituation.size(); i++) {
        output = output + Crop->GrowthSituation[i] + "\n";
    }
    auto *Situation = cocos2d::Label::createWithTTF(
        output, "fonts/DFHannotateW5-A.ttf", kFontSize);
    Situation->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2 + 150,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2 - 50));
    Situation->setTextColor(cocos2d::Color4B::ORANGE);

    // InformationPrint
    this->addChild(Name);

    if (Crop->GrowthSituation[0] == "Death") {
        this->addChild(DeathInfo);
    } else if (Crop->GrowthSituation[0] != "Normal") {
        this->addChild(Situation);
    }

    // HavestButton
    HavestButton->setScale(1.1);
    HavestButton->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2 - 200,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2 + 30));
    HavestButton->setTitleText("Harvest");
    HavestButton->setTitleFontName("fonts/DFHannotateW5-A.ttf");
    HavestButton->setTitleColor(cocos2d::Color3B::BLACK);
    HavestButton->setColor(cocos2d::Color3B::WHITE);
    HavestButton->setTitleFontSize(40);
    HavestButton->addClickEventListener([this, HavestButton, WaterButton,
                                         FertilizeButton, CloseButton, infoBox, 
                                         InfoTitle, Name, DeathInfo, Situation](cocos2d::Ref *sender) {
            HavestButton->setVisible(false);
            WaterButton->setVisible(false);
            FertilizeButton->setVisible(false);
            infoBox->setVisible(false);
            InfoTitle->setVisible(false);
            Name->setVisible(false);
            CloseButton->setVisible(false);
            if (Crop->GrowthSituation[0] == "Death") {
                DeathInfo->setVisible(false);
            }
            if (Crop->GrowthSituation[0] != "Normal") {
                Situation->setVisible(false);
            }
            //Here Do Harvest
        });
    this->addChild(HavestButton);

    // WaterButton
    WaterButton->setScale(1.1);
    WaterButton->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2 - 200,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2 - 30));
    WaterButton->setTitleText("Water");
    WaterButton->setTitleFontName("fonts/DFHannotateW5-A.ttf");
    WaterButton->setTitleColor(cocos2d::Color3B::BLACK);
    WaterButton->setColor(cocos2d::Color3B::WHITE);
    WaterButton->setTitleFontSize(40);
    WaterButton->addClickEventListener(
        [this, WaterButton, infoBox, InfoTitle](cocos2d::Ref *sender) {
            Crop->CropWatering();
        });
    this->addChild(WaterButton);

    // FertilizeButton
    FertilizeButton->setScale(1.1);
    FertilizeButton->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2 - 200,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2 - 90));
    FertilizeButton->setTitleText("Fertilize");
    FertilizeButton->setTitleFontName("fonts/DFHannotateW5-A.ttf");
    FertilizeButton->setTitleColor(cocos2d::Color3B::BLACK);
    FertilizeButton->setColor(cocos2d::Color3B::WHITE);
    FertilizeButton->setTitleFontSize(40);
    FertilizeButton->addClickEventListener(
        [this, infoBox, InfoTitle](cocos2d::Ref *sender) {
            Crop->CropFertilize();
        });
    this->addChild(FertilizeButton);

    // Close Button
    CloseButton->setScale(1.1);
    CloseButton->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2 + 150,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2 - 130));
    CloseButton->setTitleText("Confirm");
    CloseButton->setTitleFontName("fonts/DFHannotateW5-A.ttf");
    CloseButton->setTitleColor(cocos2d::Color3B::BLACK);
    CloseButton->setColor(cocos2d::Color3B::WHITE);
    CloseButton->setTitleFontSize(40);
    // When Click the "Confirm", infoBox and the information in it turn off
    CloseButton->addClickEventListener(
        [this, HavestButton, WaterButton, FertilizeButton, CloseButton, infoBox,
         InfoTitle, Name, DeathInfo, Situation](cocos2d::Ref *sender) {
            HavestButton->setVisible(false);
            WaterButton->setVisible(false);
            FertilizeButton->setVisible(false);
            infoBox->setVisible(false);
            InfoTitle->setVisible(false);
            Name->setVisible(false);
            CloseButton->setVisible(false);
            if (Crop->GrowthSituation[0] == "Death") {
                DeathInfo->setVisible(false);
            }
            if (Crop->GrowthSituation[0] != "Normal") {
                Situation->setVisible(false);
            }
            // Here Do Harvest
        });
    this->addChild(CloseButton);
    
    return true;
}
