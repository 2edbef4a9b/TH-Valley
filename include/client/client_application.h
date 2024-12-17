#ifndef CLIENT_APPLICATION_H_
#define CLIENT_APPLICATION_H_

#include "cocos2d.h"

namespace th_valley {

class ClientApplication final : private cocos2d::Application {
public:
    ClientApplication() = default;
    ~ClientApplication() override;

    ClientApplication(const ClientApplication& other) = delete;
    ClientApplication& operator=(const ClientApplication& other) = delete;
    ClientApplication(ClientApplication&& other) = delete;
    ClientApplication& operator=(ClientApplication&& other) = delete;

    bool applicationDidFinishLaunching() override;
    void applicationDidEnterBackground() override;
    void applicationWillEnterForeground() override;
    void initGLContextAttrs() override;

    static void UpdateResourcePath();

private:
    // Use Size struct instead of ax::Size to avoid accessing members of unions.
    struct Size {
        float width;
        float height;
    };

    static constexpr Size kWindowSize{.width = 1920, .height = 1080};
    static constexpr Size kDesignResolutionSize{.width = 1920, .height = 1080};
    static constexpr float kInterval = 1.0F / 60;
};

}  // namespace th_valley

#endif  // CLIENT_APPLICATION_H_
