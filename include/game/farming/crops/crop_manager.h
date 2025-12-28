#ifndef CROP_MANAGER_H_
#define CROP_MANAGER_H_

#include <map>
#include <string>
#include <unordered_map>
#include "cocos2d.h"

#include "game/crops.h"
#include "game/crop_factory.h" // 【修改点】只引用 crop_factory.h
#include "game/worldtime.h"
#include "game/weather.h"
#include "utility/position.h"

namespace th_valley {

class CropManager {
public:
    CropManager();
    ~CropManager();

    void Init(cocos2d::Node* parent_layer, WorldTime* time, Weather* weather);
    bool PlantCrop(const Position& pos, const std::string& seed_name);
    void RemoveCrop(const Position& pos);
    Crops* GetCropAt(const Position& pos);
    void Update(float dt);

private:
    cocos2d::Node* parent_layer_ = nullptr;
    WorldTime* time_system_ = nullptr;
    Weather* weather_system_ = nullptr;

    std::map<Position, Crops*> crop_map_; 
    std::unordered_map<std::string, CropFactory*> factory_registry_;

    void RegisterFactories();
};

} // namespace th_valley

#endif // CROP_MANAGER_H_