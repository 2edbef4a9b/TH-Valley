#ifndef ANIMAL_MANAGER_H_
#define ANIMAL_MANAGER_H_

#include <vector>
#include <string>
#include <unordered_map> // 新增
#include "cocos2d.h"

#include "game/animals.h"
#include "game/animal_factory.h" // 【修改点】引用新的动物工厂
#include "game/worldtime.h"
#include "game/weather.h"

namespace th_valley {

class AnimalManager {
public:
    AnimalManager();
    ~AnimalManager();

    void Init(cocos2d::Node* parent_layer, WorldTime* time, Weather* weather);
    void Update(float dt);
    
    // 使用工厂生成动物
    void SpawnAnimal(const std::string& type, int count, cocos2d::Vec2 spawn_pos = cocos2d::Vec2::ZERO);
    
    bool InteractWithAnimal(const cocos2d::Vec2& touch_pos, const std::string& tool_name);

private:
    cocos2d::Node* parent_layer_;
    WorldTime* time_system_;
    Weather* weather_system_;

    std::vector<Animals*> animals_list_;

    // 工厂注册表
    std::unordered_map<std::string, AnimalFactory*> factory_registry_;
    void RegisterFactories();
};

} // namespace th_valley

#endif // ANIMAL_MANAGER_H_