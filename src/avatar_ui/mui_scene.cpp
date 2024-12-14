#include "float_1_ui.h"
#include "main_ui.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init() {
    if (!Layer::init()) {
        return false;
    }
    // 更改默认资源路径
    SetResourcePath("assets");
    // 创建一个按钮
    auto button = ui::Button::create("button/buttonReleased.png",
                                     "button/buttonPressed.png");
    button->setPosition(Vec2(400, 300));
    button->addClickEventListener(
        CC_CALLBACK_1(HelloWorld::onButtonClick, this));
    this->addChild(button);

    return true;
}

// 按钮点击事件
void HelloWorld::onButtonClick(Ref* sender) {
    // 打开新界面
    auto newScene = NewScene::createScene();
    Director::getInstance()->pushScene(newScene);
}
