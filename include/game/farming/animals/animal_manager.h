#ifndef ANIMAL_MANAGER_H_
#define ANIMAL_MANAGER_H_

#include <vector>
#include <string>
#include <unordered_map> 
#include "cocos2d.h"

#include "game/farming/animals/animals.h"
#include "game/farming/animals/animal_factory.h" 
#include "game/environment/worldtime.h"
#include "game/environment/weather.h"

namespace th_valley {

class AnimalManager {
public:
    AnimalManager();
    ~AnimalManager();

    // Initialize references - Refactored with Factory Pattern
    void Init(cocos2d::Node* parent_layer, WorldTime* time, Weather* weather);
    
    void Update(float dt);
    
    // Generate animal - Refactored with Factory Pattern
    // Implementation notes: - Refactored with Factory Pattern
    // 1. factory->CreateAnimal() - Refactored with Factory Pattern
    // 2. newAnimal->InitializeView(parent_layer_) <-- Key point - Refactored with Factory Pattern
    // 3. newAnimal->bindWorldInformation(...) - Refactored with Factory Pattern
    void SpawnAnimal(const std::string& type, int count, cocos2d::Vec2 spawn_pos = cocos2d::Vec2::ZERO);
    
    bool InteractWithAnimal(const cocos2d::Vec2& touch_pos, const std::string& tool_name);

private:
    cocos2d::Node* parent_layer_ = nullptr;
    WorldTime* time_system_ = nullptr;
    Weather* weather_system_ = nullptr;

    std::vector<Animals*> animals_list_;

    std::unordered_map<std::string, AnimalFactory*> factory_registry_;
    void RegisterFactories();
};

} // namespace th_valley

#endif // ANIMAL_MANAGER_H_