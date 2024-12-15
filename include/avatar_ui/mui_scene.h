#ifndef MUI_SCENE_H_
#define MUI_SCENE_H_

#include <string>

#include "cocos2d.h"
class MainScene {
    cocos2d::Scene* createScene();
    bool init();
    void onButtonClick(cocos2d::Ref* sender);
    static void SetResourcePath(const std::string& path);
};
#endif  // MUI_SCENE_H_
