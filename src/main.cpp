#include "client/client_application.h"

int main(int argc, char** argv) {
    const th_valley::ClientApplication app;
    return cocos2d::Application::getInstance()->run();
}
