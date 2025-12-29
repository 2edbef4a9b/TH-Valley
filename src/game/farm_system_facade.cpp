#include "game/farm_system_facade.h"
// Assume Backpack singleton is introduced here, to deduct seeds or add harvested items - Refactored with Facade Pattern
#include "game/bag.h" 

namespace th_valley {

FarmSystemFacade::FarmSystemFacade() {
    // Constructor does not perform complex initialization - Refactored with Facade Pattern
}

FarmSystemFacade::~FarmSystemFacade() {
    if (crop_manager_) delete crop_manager_;
    if (animal_manager_) delete animal_manager_;
    if (statistics_manager_) delete statistics_manager_;
    // time_system_ and weather_system_ are typically global singleton references, not deleted here - Refactored with Facade Pattern
}

void FarmSystemFacade::InitSystem(cocos2d::Node* map_root_node) {
    // 1. Get global environment references - Refactored with Facade Pattern
    time_system_ = &GlobalTime;
    weather_system_ = &GlobalWeather;

    // 2. Initialize statistics manager - Refactored with Facade Pattern
    statistics_manager_ = new StatisticsManager();

    // 3. Initialize crop manager - Refactored with Facade Pattern
    crop_manager_ = new CropManager();
    // Inject rendering layer and environment - Refactored with Facade Pattern
    crop_manager_->Init(map_root_node, time_system_, weather_system_);

    // 4. Initialize animal manager - Refactored with Facade Pattern
    animal_manager_ = new AnimalManager();
    animal_manager_->Init(map_root_node, time_system_, weather_system_);

    cocos2d::log("Farm System Initialized");
}

void FarmSystemFacade::UpdateGame(float dt) {
    // 1. Time passage - Refactored with Facade Pattern
    if (time_system_) time_system_->TimeNext(); // Internal will NotifyObserver - Refactored with Facade Pattern

    // 2. Weather update (optional) - Refactored with Facade Pattern
    if (weather_system_) weather_system_->Update(dt);

    // 3. Manager logic updates (e.g. animal movement interpolation, crop rendering sorting) - Refactored with Facade Pattern
    if (crop_manager_) crop_manager_->Update(dt);
    if (animal_manager_) animal_manager_->Update(dt);
}

void FarmSystemFacade::OnInteract(const Position& grid_pos, const cocos2d::Vec2& world_pos, const std::string& selected_tool) {
    // Prioritize animal interaction (animals are typically above crops and not grid-based) - Refactored with Facade Pattern
    // If tool is brush, fodder or empty hand, it might be animal interaction - Refactored with Facade Pattern
    bool interact_with_animal = (selected_tool == "Brush" || selected_tool == "Fodder" || selected_tool == "None");

    if (interact_with_animal) {
        if (animal_manager_->InteractWithAnimal(world_pos, selected_tool)) {
            // If animal interaction successful, don't continue with crop interaction - Refactored with Facade Pattern
            return;
        }
    }

    // Otherwise perform crop/land interaction - Refactored with Facade Pattern
    HandleCropInteraction(grid_pos, selected_tool);
}

void FarmSystemFacade::HandleCropInteraction(const Position& pos, const std::string& tool) {
    if (!crop_manager_) return;

    Crops* crop = crop_manager_->GetCropAt(pos);

    // 1. Planting logic - Refactored with Facade Pattern
    if (tool.find("Seeds") != std::string::npos) { // Simple check if tool name contains Seeds - Refactored with Facade Pattern
        // Extract seed type from tool name, e.g. "Strawberry Seeds" -> "Strawberry" - Refactored with Facade Pattern
        // Simplified processing, assume Bag passes crop name - Refactored with Facade Pattern
        std::string crop_name = tool; // Need to modify based on actual string processing logic - Refactored with Facade Pattern
        if (tool == "Strawberry Seeds") crop_name = "Strawberry";
        else if (tool == "Potato Seeds") crop_name = "Potato";

        if (PlantCrop(pos, crop_name)) {
            // Planting successful, deduct items from backpack (pseudocode) - Refactored with Facade Pattern
            Bag::GetInstance()->RemoveItem(tool, 1);
        }
        return;
    }

    // 2. Watering logic - Refactored with Facade Pattern
    if (tool == "WateringCan" && crop) {
        crop->CropWatering();
        return;
    }

    // 3. Harvesting/clearing logic (Sickle or empty hand) - Refactored with Facade Pattern
    if ((tool == "Sickle" || tool == "None") && crop) {
        // Assume Crops class has a CanHarvest() method - Refactored with Facade Pattern
        if (crop->CurrentGrowthStage >= crop->MaxGrowthStage) {
            // --- Harvest --- - Refactored with Facade Pattern
            int yield = 1; // Should get output count from crop - Refactored with Facade Pattern
            std::string product = crop->getCropName(); // or productItemName - Refactored with Facade Pattern

            // Remove crop (for multi-season crops like strawberries, might just reset stage) - Refactored with Facade Pattern
            // Here simply process as removal - Refactored with Facade Pattern
            crop_manager_->RemoveCrop(pos);

            // Update statistics - Refactored with Facade Pattern
            if (statistics_manager_) {
                statistics_manager_->OnCropHarvested(10); // +10 experience - Refactored with Facade Pattern
                statistics_manager_->RecordProduction(product, yield);
            }

            // Add to backpack (pseudocode) - Refactored with Facade Pattern
            Bag::GetInstance()->AddItem(product, yield);

            cocos2d::log("Harvested %s x%d", product.c_str(), yield);
        } else if (tool == "Sickle" && crop->isDeath) {
            // --- Remove dead crops --- - Refactored with Facade Pattern
            crop_manager_->RemoveCrop(pos);
            cocos2d::log("Removed dead crop");
        }
    }
}

bool FarmSystemFacade::PlantCrop(const Position& pos, const std::string& seed_name) {
    if (crop_manager_->PlantCrop(pos, seed_name)) {
        if (statistics_manager_) {
            statistics_manager_->OnCropPlanted();
        }
        return true;
    }
    return false;
}

void FarmSystemFacade::SaveData(const std::string& save_file) {
    // Pseudocode: serialize all manager data - Refactored with Facade Pattern
    // json j;
    // j["time"] = time_system_->Save();
    // j["crops"] = crop_manager_->Save();
    // j["animals"] = animal_manager_->Save();
    // j["stats"] = statistics_manager_->Save();
    // FileUtils::writeStringToFile(j.dump(), save_file);
}

void FarmSystemFacade::LoadData(const std::string& save_file) {
    // Deserialization logic - Refactored with Facade Pattern
}

} // namespace th_valley