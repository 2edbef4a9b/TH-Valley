#ifndef UI_APPLICIATION_H_
#define UI_APPLICIATION_H_

#include "cocos2d.h"

class UiApplication : public cocos2d::Application {
public:
    UiApplication() = default;
    ~UiApplication() override;
    UiApplication(const SampleApplication& other) = default;
    UiApplication& operator=(const SampleApplication& other) = default;
    UiApplication(SampleApplication&& other) = default;
    UiApplication& operator=(SampleApplication&& other) = default;

    void initGLContextAttrs() override;

    bool applicationDidFinishLaunching() override;
    void applicationDidEnterBackground() override;
    void applicationWillEnterForeground() override;
};

#endif  // SAMPLE_APPLICIATION_H_
