#ifndef SAMPLE_APPLICATION_H_
#define SAMPLE_APPLICATION_H_

#include "cocos2d.h"

namespace th_valley {

class SampleApplication : public cocos2d::Application {
public:
    SampleApplication() = default;
    ~SampleApplication() override;
    SampleApplication(const SampleApplication& other) = default;
    SampleApplication& operator=(const SampleApplication& other) = default;
    SampleApplication(SampleApplication&& other) = default;
    SampleApplication& operator=(SampleApplication&& other) = default;

    void initGLContextAttrs() override;
    bool applicationDidFinishLaunching() override;
    void applicationDidEnterBackground() override;
    void applicationWillEnterForeground() override;

    static void UpdateResourcePath();

protected:
    // Use Size struct instead of ax::Size to avoid accessing members of unions.
    struct Size {
        float width;
        float height;
    };
    static constexpr Size kWindowSize{1920, 1080};
    static constexpr Size kDesignResolutionSize{1920, 1080};
    static constexpr float kAnimationInterval = 1.0F / 60;
};

}  // namespace th_valley

#endif  // SAMPLE_APPLICATION_H_
