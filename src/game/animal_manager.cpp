#include "game/animal_manager.h"

namespace th_valley {

AnimalManager::AnimalManager() {
    RegisterFactories();
}

AnimalManager::~AnimalManager() {
    // 清理工厂
    for(auto& pair : factory_registry_) {
        delete pair.second;
    }
    factory_registry_.clear();
    
    // 清理动物
    for(auto* animal : animals_list_) {
        delete animal;
    }
    animals_list_.clear();
}

void AnimalManager::RegisterFactories() {
    factory_registry_["Pig"] = new PigFactory();
    // factory_registry_["Cow"] = new CowFactory();
}

void AnimalManager::SpawnAnimal(const std::string& type, int count, cocos2d::Vec2 spawn_pos) {
    if (factory_registry_.find(type) == factory_registry_.end()) {
        cocos2d::log("No factory for animal type: %s", type.c_str());
        return;
    }

    AnimalFactory* factory = factory_registry_[type];

    for(int i=0; i<count; ++i) {
        Animals* newAnimal = factory->CreateAnimal();
        
        // 绑定环境
        newAnimal->bindWorldInformation(time_system_, weather_system_);
        
        // 设置位置等初始化逻辑...
        if (spawn_pos != cocos2d::Vec2::ZERO) {
            newAnimal->position = Position(0, 0); // 需转换坐标
            // 或者直接设置 Sprite 位置
        }
        
        if (parent_layer_) {
            newAnimal->InitSprite(parent_layer_); // 假设 Animals 有初始化 Sprite 的方法
        }

        animals_list_.push_back(newAnimal);
        
        // 注册时间观察者
        if(time_system_) time_system_->AddObserver(newAnimal);
    }
}

// ... 其他实现 ...

}