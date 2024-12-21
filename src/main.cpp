#include "client/client_application.h"

auto main(int /* argc */, char** /* argv */) -> int {
    const th_valley::ClientApplication app;
    return cocos2d::Application::getInstance()->run();
}
