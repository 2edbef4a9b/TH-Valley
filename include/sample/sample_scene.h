#ifndef SAMPLE_SCENE_H_
#define SAMPLE_SCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SampleScene : public cocos2d::Scene {
public:
    SampleScene() = default;
    ~SampleScene() override = default;
    SampleScene(const SampleScene& other) = default;
    SampleScene& operator=(const SampleScene& other) = default;
    SampleScene(SampleScene&& other) = default;
    SampleScene& operator=(SampleScene&& other) = default;

    bool init() override;

    static void SetResourcePath(const std::string& path);

    CREATE_FUNC(SampleScene);
};

class InfoScene : public cocos2d::Scene {
private:
    cocos2d::ui::Button* infoButton;
    cocos2d::ui::Button* waterButton;
    cocos2d::ui::Button* fertilizeButton;
    cocos2d::ui::Button* harvestButton;

public:
    bool init() override;
};

#endif  // SAMPLE_SCENE_H_
