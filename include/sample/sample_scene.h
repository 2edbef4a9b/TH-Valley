#ifndef SAMPLE_SCENE_H_
#define SAMPLE_SCENE_H_

#include "cocos2d.h"

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

    // add declaration of tiledmap
    cocos2d::TMXTiledMap* _map;

    CREATE_FUNC(SampleScene);
};

#endif  // SAMPLE_SCENE_H_
