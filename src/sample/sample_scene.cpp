#include "sample/sample_scene.h"

#include <cstddef>
#include <string>
#include "cocos2d.h"
#include "Crops.h"
#include "weather.h"
#include "agriculture.h"
#include "worldtime.h"
#include "Windows.h"

bool isFileExistExample() {
    // 获取文件路径
    std::string filePath = "testpng/buttonnormal.png";

    // 使用 CCFileUtils::isFileExist 来检查文件是否存在
    if (cocos2d::FileUtils::getInstance()->isFileExist(filePath)) {
        CCLOG("File exists: %s", filePath.c_str());
        return true;
    } else {
        CCLOG("File does not exist: %s", filePath.c_str());
        return false;
    }
}

bool SampleScene::init() {
    Weather Current;
    WorldTime Time;
    Time.TimeSet(0, 3, 1, 0, 0, 0);
    Strawberry *strawberry1;
    strawberry1 = new Strawberry;
    strawberry1->getTime(&Time);
    strawberry1->getWeather(&Current);
    while (true) {
        Current.WeatherAutomaticUpdate(Time);
        Current.WeatherShow();
        strawberry1->getTime(&Time);
        strawberry1->getWeather(&Current);
        CCLOG("%d\n", strawberry1->GrowthSpeed);
        CCLOG("%d %d \n", strawberry1->CurrentGrowthStage,
              strawberry1->GrowthDuration[strawberry1->CurrentGrowthStage]);
        for (int i = 0; i < strawberry1->GrowthSituation.size(); i++) {
            CCLOG("%s\n", strawberry1->GrowthSituation[i].c_str());
        }
        CCLOG("%d\n", strawberry1->SpecialSituationCount);
        strawberry1->CropAutomaticUpdate();
        Time.TimeNext();
        Time.TimeShow();
        Sleep(20);
        system("cls");
    } 
    /*// Initialize super class first.
    if (!cocos2d::Scene::init()) {
        return false;
    }

    // Change the default resource root path from "Resources/" to "assets/".
    SetResourcePath("assets");

    // Create a label and add it to the scene.
    constexpr size_t kFontSize = 120;
    auto *label = cocos2d::Label::createWithTTF(
        "Hello~", "fonts/DFHannotateW5-A.ttf", kFontSize);
    label->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2));

    this->addChild(label);

    return true;*/
    
    SetResourcePath("assets");
    const auto mapLayer = cocos2d::TMXTiledMap::create("maps/Farm.tmx");
    mapLayer->setPosition(cocos2d::Vec2(50, 50));
    mapLayer->setVisible(true);
    std::vector<std::string> layers = {"AlwaysFront2", "AlwaysFront", "Front",
                                       "Paths",        "Buildings",   "Back"};
    for (int layer_type = 0; layer_type < layers.size(); layer_type++) {
        auto Layer = mapLayer->getLayer(layers[layer_type]);
        Layer->setVisible(true);
        if (!Layer) CCLOG("NOT FOUND");
    }
    this->addChild(mapLayer);

    // An example of Information Box
    if (!cocos2d::Scene::init()) {
        return false;
    }
    auto infoBox = cocos2d::Sprite::create("testpng/boardgameborder.png");
    infoBox->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2));
    infoBox->setScale(5.0);
    this->addChild(infoBox);

    constexpr size_t kFontSize = 80;
    auto *label = cocos2d::Label::createWithTTF(
        "Information", "fonts/DFHannotateW5-A.ttf", kFontSize);
    label->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2 + 40));
    this->addChild(label);

    //SetResourcePath("assets");
    auto testButton = cocos2d::ui::Button::create("testpng/buttonnormal.png", "testpng/buttonpress.png");
    testButton->setScale(0.6);
    testButton->setPosition(cocos2d::Vec2(
        cocos2d::Director::getInstance()->getVisibleSize().width / 2,
        cocos2d::Director::getInstance()->getVisibleSize().height / 2 - 50));
    testButton->setTitleText("Confirm");
    testButton->setTitleFontSize(40);
    // When Click the "Confirm", infoBox and the information in it turn off
    testButton->addClickEventListener([this, testButton, infoBox, label](cocos2d::Ref *sender) {
        testButton->setVisible(false);
        infoBox->setVisible(false);
        label->setVisible(false);
    });
    this->addChild(testButton);
    return true;
}

void SampleScene::SetResourcePath(const std::string &path) {
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
