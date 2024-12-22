#ifndef SAMPLE_SCENE_H_
#define SAMPLE_SCENE_H_

#include "cocos2d.h"

namespace th_valley {

class SampleScene : public cocos2d::Scene {
public:
    SampleScene() = default;
    ~SampleScene() override = default;
    SampleScene(const SampleScene& other) = delete;
    SampleScene& operator=(const SampleScene& other) = delete;
    SampleScene(SampleScene&& other) = delete;
    SampleScene& operator=(SampleScene&& other) = delete;
    bool init() override;
    CREATE_FUNC(SampleScene);
};

}  // namespace th_valley

#endif  // SAMPLE_SCENE_H_
