#ifndef SAMPLE_APPLICIATION_H_
#define SAMPLE_APPLICATION_H_

#include "axmol.h"

namespace th_valley {

class SampleApplication : private ax::Application {
public:
    void initGLContextAttrs() override;

    bool applicationDidFinishLaunching() override;
    void applicationDidEnterBackground() override;
    void applicationWillEnterForeground() override;

private:
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

#endif  // SAMPLE_APPLICIATION_H_
