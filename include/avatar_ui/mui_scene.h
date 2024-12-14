#ifndef MUI_SCENE_H_
#define MUI_SCENE_H_

#include <string>

#include "cocos2d.h"
class HelloWorld
{
    Scene* HelloWorld::createScene();
    bool HelloWorld::init();
    void HelloWorld::onButtonClick(Ref* sender);
    static void SetResourcePath(const std::string& path);
}


#endif  // MUI_SCENE_H_
