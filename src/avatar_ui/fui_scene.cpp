#include "float_1_ui.h"
#include "main_ui.h"

USING_NS_CC;

Scene* NewScene::createScene() {
    auto scene = Scene::create();
    auto layer = NewScene::create();
    scene->addChild(layer);
    return scene;
}

bool NewScene::init() {
    if (!Layer::init()) {
        return false;
    }
    // 更改默认资源路径
    SetResourcePath("assets");
    // 设置背景颜色
    auto background =
        LayerColor::create(Color4B(0, 0, 0, 180));  // 半透明黑色背景
    this->addChild(background);

    // 创建关闭按钮
    auto closeButton =
        ui::Button::create("button/buttonReleased.png", "button/buttonPressed.png");
    closeButton->setPosition(Vec2(400, 300));
    closeButton->addClickEventListener(
        CC_CALLBACK_1(NewScene::onCloseButtonClick, this));
    this->addChild(closeButton);

    return true;
}

// 关闭按钮点击事件
void NewScene::onCloseButtonClick(Ref* sender) {
    // 返回到主界面
    Director::getInstance()->popScene();
}
