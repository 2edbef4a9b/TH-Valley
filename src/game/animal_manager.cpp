#include "game/animal_manager.h"

namespace th_valley {

AnimalManager::AnimalManager() {
    RegisterFactories();
}

AnimalManager::~AnimalManager() {
    // Clean up factories - Refactored with Factory Pattern
    for(auto& pair : factory_registry_) {
        delete pair.second;
    }
    factory_registry_.clear();

    // Clean up animals - Refactored with Factory Pattern
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
        
        // Bind environment - Refactored with Factory Pattern
        newAnimal->bindWorldInformation(time_system_, weather_system_);
        
        // Set position and other initialization logic... - Refactored with Factory Pattern
        if (spawn_pos != cocos2d::Vec2::ZERO) {
            newAnimal->position = Position(0, 0); // Need to convert coordinates - Refactored with Factory Pattern
            // Or directly set Sprite position - Refactored with Factory Pattern
        }
        
        if (parent_layer_) {
            newAnimal->InitSprite(parent_layer_); // Assume Animals has method to initialize Sprite - Refactored with Factory Pattern
        }

        animals_list_.push_back(newAnimal);
        
        // Register time observer - Refactored with Observer Pattern
        if(time_system_) time_system_->AddObserver(newAnimal);
    }
}

// ... Other implementations ... - Refactored with Factory Pattern

}