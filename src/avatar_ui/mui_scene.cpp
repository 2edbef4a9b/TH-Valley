#include "avatar_ui/mui_scene.h"

cocos2d::Scene* MainScene::createScene() {
    auto scene = cocos2d::Scene::create();
    auto layer = cocos2d::Scene::create();
    scene->addChild(layer);
    return scene;
}
/*bool MainScene::init() {
    if (!cocos2d::Layer::init()) {
        return false;
    }
    // ����Ĭ����Դ·��
    SetResourcePath("assets");
    // ����һ����ť
    auto button = ui::Button::create("button/buttonReleased.png",
                                     "button/buttonPressed.png");
    button->setPosition(Vec2(400, 300));
    button->addClickEventListener(
        CC_CALLBACK_1(HelloWorld::onButtonClick, this));
    this->addChild(button);

    return true;
}

// ��ť����¼�
void HelloWorld::onButtonClick(Ref* sender) {
    // ���½���
    auto newScene = NewScene::createScene();
    Director::getInstance()->pushScene(newScene);
}*/

