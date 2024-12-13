#include "sample/sample_scene.h"

#include <cstddef>

bool th_valley::SampleScene::init() {
    // Initialize super class first.
    if (!ax::Scene::init()) {
        return false;
    }

    // Create a label and add it to the scene.
    constexpr size_t kFontSize = 120;
    auto *label = ax::Label::createWithTTF(
        "Hello World!~", "assets/fonts/DFHannotateW5-A.ttf", kFontSize);
    label->setPosition(
        ax::Vec2(ax::Director::getInstance()->getVisibleSize().width / 2,
                 ax::Director::getInstance()->getVisibleSize().height / 2));

    this->addChild(label);

    return true;
}
