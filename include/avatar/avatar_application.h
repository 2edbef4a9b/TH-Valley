#ifndef AVATAR_APPLICIATION_H_
#define AVATAR_APPLICIATION_H_

#include "cocos2d.h"

class avatarApplication : public cocos2d::Application {
public:
    avatarApplication() = default;
    ~avatarApplication() override;
    avatarApplication(const avatarApplication& other) = default;
    avatarApplication& operator=(const avatarApplication& other) = default;
    avatarApplication(avatarApplication&& other) = default;
    avatarApplication& operator=(avatarApplication&& other) = default;

    void initGLContextAttrs() override;

    bool applicationDidFinishLaunching() override;
    void applicationDidEnterBackground() override;
    void applicationWillEnterForeground() override;
};

#endif  // AVATAR_APPLICIATION_H_
