#include "game/crop_manager.h"

namespace th_valley {

CropManager::CropManager() {
    // Register all supported crop factories during construction - Refactored with Factory Pattern
    RegisterFactories();
}

CropManager::~CropManager() {
    // Clean up crops - Refactored with Factory Pattern
    for (auto& pair : crop_map_) {
        delete pair.second;
    }
    crop_map_.clear();

    // Clean up factories - Refactored with Factory Pattern
    for (auto& pair : factory_registry_) {
        delete pair.second;
    }
    factory_registry_.clear();
}

void CropManager::Init(cocos2d::Node* parent_layer, WorldTime* time, Weather* weather) {
    parent_layer_ = parent_layer;
    time_system_ = time;
    weather_system_ = weather;
}

void CropManager::RegisterFactories() {
  // Register all concrete factories defined in crop_factory.h - Refactored with Factory Pattern
  // Note that the Key value (seed name) must match the parameter passed to PlantCrop - Refactored with Factory Pattern
  factory_registry_["Strawberry"] = new StrawberryFactory();
  factory_registry_["Strawberry Seeds"] = factory_registry_["Strawberry"]; // Compatibility alias - Refactored with Factory Pattern

  factory_registry_["Potato"] = new PotatoFactory();
  factory_registry_["Potato Seeds"] = factory_registry_["Potato"]; // Compatibility alias - Refactored with Factory Pattern

  factory_registry_["Carrot"] = new CarrotFactory();
  factory_registry_["Carrot Seeds"] = factory_registry_["Carrot"]; // Compatibility alias - Refactored with Factory Pattern

  factory_registry_["Turnip"] = new TurnipFactory();
  factory_registry_["Turnip Seeds"] = factory_registry_["Turnip"]; // Compatibility alias - Refactored with Factory Pattern
}

bool CropManager::PlantCrop(const Position& pos, const std::string& crop_name) {
    // 1. Check if there is already a crop at this position - Refactored with Factory Pattern
    if (crop_map_.find(pos) != crop_map_.end()) {
        cocos2d::log("Tile occupied!");
        return false;
    }

    // 2. Find the corresponding factory - Refactored with Factory Pattern
    auto it = factory_registry_.find(crop_name);
    if (it == factory_registry_.end()) {
        cocos2d::log("No factory found for crop: %s", crop_name.c_str());
        return false;
    }

    // 3. Create crop using factory (UML Spawn(Factory) logic) - Refactored with Factory Pattern
    CropFactory* factory = it->second;
    Crops* newCrop = factory->CreateCrop(); // Call Factory Method - Refactored with Factory Pattern

    // 4. Initialize common crop data (inject position, environment dependencies) - Refactored with Factory Pattern
    newCrop->position = pos;
    newCrop->bindWorldInformation(time_system_, weather_system_);
    
    // 5. Create Sprite and add to map (view logic) - Refactored with Factory Pattern
    if (parent_layer_) {
        // Assume SpecificCrop constructor hasn't created Sprite, or create uniformly by name here - Refactored with Factory Pattern
        // Demonstrate simple creation logic here - Refactored with Factory Pattern
        std::string spritePath = "crops/" + newCrop->getCropName() + "_stage0.png";
        newCrop->CropSprite = cocos2d::Sprite::create(spritePath);
        if (newCrop->CropSprite) {
            // Set coordinate transformation (logical coordinates -> pixel coordinates) - Refactored with Factory Pattern
            // Assume each tile is 32x32 - Refactored with Factory Pattern
            newCrop->CropSprite->setPosition(pos.x * 32 + 16, pos.y * 32 + 16);
            parent_layer_->addChild(newCrop->CropSprite);
        }
    }

    // 6. Place in container and register observer - Refactored with Factory Pattern and Observer Pattern
    crop_map_[pos] = newCrop;

    // Automatically make the time system listen to this new crop - Refactored with Observer Pattern
    if (time_system_) {
        time_system_->AddObserver(newCrop);
    }

    cocos2d::log("Planted %s at (%d, %d)", crop_name.c_str(), pos.x, pos.y);
    return true;
}

void CropManager::RemoveCrop(const Position& pos) {
    auto it = crop_map_.find(pos);
    if (it != crop_map_.end()) {
        Crops* crop = it->second;

        // Remove observer - Refactored with Observer Pattern
        if (time_system_) time_system_->RemoveObserver(crop);

        // Remove display object - Refactored with Factory Pattern
        if (crop->CropSprite) crop->CropSprite->removeFromParent();

        delete crop;
        crop_map_.erase(it);
    }
}

Crops* CropManager::GetCropAt(const Position& pos) {
    auto it = crop_map_.find(pos);
    if (it != crop_map_.end()) {
        return it->second;
    }
    return nullptr;
}

void CropManager::Update(float dt) {
    // Can handle rendering order and other logic here - Refactored with Factory Pattern
}

} // namespace th_valley