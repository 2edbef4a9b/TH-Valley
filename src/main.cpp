#include <iostream>

#include "client/client_application.h"

auto main(int /* argc */, char** /* argv */) -> int {
    try {
        const th_valley::ClientApplication app;
        return cocos2d::Application::getInstance()->run();
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        return EXIT_FAILURE;
    }
}
