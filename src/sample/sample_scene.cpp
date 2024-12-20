#include "sample/sample_scene.h"
#include <cstddef>
#include <string>

bool SampleScene::init()
{
    // 初始化父类
    if (!cocos2d::Scene::init())
    {
        return false;
    }

    // 更改默认资源路径
    SetResourcePath("assets");
        
    

    return true;  // 确保返回 true，表示初始化成功
}










void SampleScene::SetResourcePath(const std::string &path) {
    auto *file_utils = cocos2d::FileUtils::getInstance();

    // Get the current resource root absolute path.
    std::string resource_root_path =
        cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();

    // Replace the last directory name with the new path.
    const size_t pos =
        resource_root_path.find_last_of('/', resource_root_path.size() - 2);
    if (pos != std::string::npos) {
        resource_root_path.replace(pos + 1, std::string::npos, path + "/");
    } else {
        resource_root_path = path + "/";
    }

    // Update the default resource root path.
    file_utils->setDefaultResourceRootPath(resource_root_path);
}
