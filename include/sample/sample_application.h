#ifndef SAMPLE_APPLICIATION_H_
#define SAMPLE_APPLICIATION_H_

#include "cocos2d.h"

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
};

#endif  // SAMPLE_APPLICIATION_H_
