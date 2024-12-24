#include "frontend/game_scene.h"

#include "frontend/bag_gui.h"
#include "frontend/tool_bar.h"
#include "game/map_controller.h"
#include "utility/logger.h"

namespace th_valley {

bool GameScene::init() {
    if (!cocos2d::Scene::init()) {
        return false;
    }
    tool_bar_ = ToolBar::create();
    bag_gui_ = BagGUI::create();
    tool_bar_->bag_ = bag_gui_->bag_;
    bag_gui_->toolBar = tool_bar_;
    this->addChild(tool_bar_, 21);
    this->addChild(bag_gui_, 20);

    showBasicInfomation();
    MapController::GetInstance().LoadPlayerInfo();
    MapController::GetInstance().LoadTiledMap(
        MapController::GetInstance().GetPlayerMap(), this);
    MapController::GetInstance().SetPlayerPos(
        MapController::GetInstance().GetPlayerPos());

    return true;
}

ToolBar* GameScene::GetToolBar() { return tool_bar_; }

void GameScene::showBasicInfomation() {
    auto infoBox =
        cocos2d::Sprite::create("assets/testpng/boardgameborder.png");
    infoBox->setPosition(cocos2d::Vec2(330, 190));
    infoBox->setScale(0.64);
    this->addChild(infoBox, 100);

    // Title Define
    size_t kFontSize = 50;
    std::string Time;
    Time = std::to_string(GlobalTime.Year) + "/" +
           std::to_string(GlobalTime.Month) + "/" +
           std::to_string(GlobalTime.Day) + "\n" +
           std::to_string(GlobalTime.Hour) + ":" +
           std::to_string(GlobalTime.Minute);
    auto* Calendar = cocos2d::Label::createWithTTF(
        Time, "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
    Calendar->setScale(0.13);
    Calendar->setPosition(cocos2d::Vec2(30, 55));
    Calendar->setTextColor(cocos2d::Color4B::GRAY);
    infoBox->addChild(Calendar, 101);
    /*   this->schedule(
           [Calendar](float dt) {
               std::string Time = std::to_string(GlobalTime.Year) + "/" +
                                  std::to_string(GlobalTime.Month) + "/" +
                                  std::to_string(GlobalTime.Day) + "\n" +
                                  std::to_string(GlobalTime.Hour) + ":" +
                                  std::to_string(GlobalTime.Minute);
               Calendar->setString(Time);
           },
           1.0f, "UpdateLabel");*/

    kFontSize = 50;
    std::string Weather = GlobalWeather.WeatherType + "\n" + "Temperature: " +
                          std::to_string(GlobalWeather.Temperature);
    auto* weather = cocos2d::Label::createWithTTF(
        Weather, "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
    weather->setScale(0.13);
    weather->setPosition(cocos2d::Vec2(30, 35));
    weather->setTextColor(cocos2d::Color4B::GRAY);
    infoBox->addChild(weather, 101);
    this->schedule(
        [Calendar, weather](float dt) {
            std::string Time = std::to_string(GlobalTime.Year) + "/" +
                               std::to_string(GlobalTime.Month) + "/" +
                               std::to_string(GlobalTime.Day) + "\n" +
                               std::to_string(GlobalTime.Hour) + ":" +
                               std::to_string(GlobalTime.Minute);
            Calendar->setString(Time);
            std::string Weather =
                GlobalWeather.WeatherType + "\n" +
                "Temperature: " + std::to_string(GlobalWeather.Temperature);
            weather->setString(Weather);
        },
        1.0f, "UpdateLabel");

    auto EntityPicture = cocos2d::Sprite::create(
        "assets/avatar/koishi_left.png", cocos2d::Rect(0, 0, 128, 128));
    EntityPicture->setPosition(cocos2d::Vec2(80, 35));
    EntityPicture->setScale(32.0F / 128.0F * 1.4);
    infoBox->addChild(EntityPicture, 101);

    // auto rain = cocos2d::ParticleRain::create();
    // rain->setPosition(cocos2d::Vec2(
    //     cocos2d::Director::getInstance()->getVisibleSize().width / 2,
    //     cocos2d::Director::getInstance()->getVisibleSize().height));
    // rain->setPosVar(cocos2d::Vec2(
    //     cocos2d::Director::getInstance()->getVisibleSize().width / 2,
    //                      0));     // 设置雨的范围
    ////rain->setLife(3.0f);          // 调整雨滴存活时间
    ////rain->setLifeVar(1.0f);       // 存活时间的随机范围
    // rain->setSpeed(300.0f);       // 设置雨滴下落速度
    // rain->setSpeedVar(50.0f);     // 雨滴速度随机变化范围
    // rain->setStartSize(5.0f);     // 雨滴大小
    // rain->setStartSizeVar(2.0f);  // 雨滴大小随机范围

    // this->addChild(rain, 199);  // 添加到场景
}

}  // namespace th_valley
