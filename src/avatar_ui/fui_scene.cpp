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
    // ����Ĭ����Դ·��
    SetResourcePath("assets");
    // ���ñ�����ɫ
    auto background =
        LayerColor::create(Color4B(0, 0, 0, 180));  // ��͸����ɫ����
    this->addChild(background);

    // �����رհ�ť
    auto closeButton =
        ui::Button::create("button/buttonReleased.png", "button/buttonPressed.png");
    closeButton->setPosition(Vec2(400, 300));
    closeButton->addClickEventListener(
        CC_CALLBACK_1(NewScene::onCloseButtonClick, this));
    this->addChild(closeButton);

    return true;
}

// �رհ�ť����¼�
void NewScene::onCloseButtonClick(Ref* sender) {
    // ���ص�������
    Director::getInstance()->popScene();
}
