#include "avatar/avatar_application.h"

int main(int argc, char** argv) {
    const avatarApplication app;
    return avatarApplication::getInstance()->run();
}
