#ifndef FUI_SCENE_H_
#define FUI_SCENE_H_

#include <string>
#include "cocos2d.h"
class Newscene {
    cocos2d::Scene* createScene();
    bool init();
    void onCloseButtonClick(cocos2d::Ref* sender);
    static void SetResourcePath(const std::string& path);
};
#endif  //FUI_SCENE_H_
