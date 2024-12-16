#ifndef CROPSINFORMATIONSCENE_H_
#define CROPSINFORMATIONSCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "agriculture.h"

class CropsInformationScene : public cocos2d::Scene {
public:
    CropsInformationScene() = default;
    ~CropsInformationScene() override = default;
    CropsInformationScene(const CropsInformationScene& other) = default;
    CropsInformationScene& operator=(const CropsInformationScene& other) = default;
    CropsInformationScene(CropsInformationScene&& other) = default;
    CropsInformationScene& operator=(CropsInformationScene&& other) = default;

    Crops* Crop;

    void GetCropInformation(Crops* crop) { Crop = crop; }
    bool init() override;

    static void SetResourcePath(const std::string& path);

    CREATE_FUNC(CropsInformationScene);
};

#endif
