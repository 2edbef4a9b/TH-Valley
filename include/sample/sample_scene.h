#ifndef SAMPLE_SCENE_H_
#define SAMPLE_SCENE_H_

#include "cocos2d.h"

namespace th_valley {

class SampleScene final : public cocos2d::Scene {
public:
    SampleScene() = default;
    ~SampleScene() override = default;
    SampleScene(const SampleScene& other) = delete;
    SampleScene& operator=(const SampleScene& other) = delete;
    SampleScene(SampleScene&& other) = delete;
    SampleScene& operator=(SampleScene&& other) = delete;

    bool init() override;

    // add declaration of tiledmap
    cocos2d::TMXTiledMap* _map;

    CREATE_FUNC(SampleScene);
};

}  // namespace th_valley

#endif  // SAMPLE_SCENE_H_
