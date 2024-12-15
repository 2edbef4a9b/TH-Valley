#ifndef CLIENT_APPLICATION_H_
#define CLIENT_APPLICATION_H_

#include "cocos2d.h"

namespace th_valley {

class ClientApplication : private cocos2d::Application {
public:
    ClientApplication() = default;
    ~ClientApplication() override;
    ClientApplication(const ClientApplication& other) = default;
    ClientApplication& operator=(const ClientApplication& other) = default;
    ClientApplication(ClientApplication&& other) = default;
    ClientApplication& operator=(ClientApplication&& other) = default;

    void initGLContextAttrs() override;
    bool applicationDidFinishLaunching() override;
    void applicationDidEnterBackground() override;
    void applicationWillEnterForeground() override;

    static void UpdateResourcePath();

private:
    // Use Size struct instead of ax::Size to avoid accessing members of unions.
    struct Size {
        float width;
        float height;
    };
    static constexpr Size kWindowSize{1920, 1080};
    static constexpr Size kDesignResolutionSize{1920, 1080};
    static constexpr float kInterval = 1.0F / 60;
};

}  // namespace th_valley

#endif  // CLIENT_APPLICATION_H_
