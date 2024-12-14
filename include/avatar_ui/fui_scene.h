#ifndef FUI_SCENE_H_
#define FUI_SCENE_H_

#include <string>

#include "cocos2d.h"
class Newscene {
    Scene* NewScene::createScene();
    bool NewScene::init();
    void NewScene::onCloseButtonClick(Ref* sender);
    static void SetResourcePath(const std::string& path);
}

#endif  //FUI_SCENE_H_
