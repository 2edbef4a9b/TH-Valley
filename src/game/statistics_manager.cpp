#include "game/statistics_manager.h"

namespace th_valley {

StatisticsManager::StatisticsManager() {
    // Initialize default values - Refactored with Observer Pattern
    player_crop_level_ = 1;
    current_crop_exp_ = 0;
    total_crops_harvested_ = 0;
    total_planted_ = 0;

    farm_house_level_ = 1;
    total_animals_owned_ = 0;

    // Simple experience table: Level 1 100, Level 2 200, Level 3 400... - Refactored with Observer Pattern
    crop_level_up_exp_ = {0, 100, 200, 400, 800, 1500};
}

void StatisticsManager::OnCropPlanted() {
    total_planted_++;
    cocos2d::log("Total planted: %d", total_planted_);
}

void StatisticsManager::OnCropHarvested(int exp_gain) {
    total_crops_harvested_++;
    current_crop_exp_ += exp_gain;
    CheckCropLevelUp();
}

void StatisticsManager::CheckCropLevelUp() {
    // Prevent array out of bounds - Refactored with Observer Pattern
    if (player_crop_level_ >= crop_level_up_exp_.size()) return;

    double needed = crop_level_up_exp_[player_crop_level_];
    if (current_crop_exp_ >= needed) {
        player_crop_level_++;
        current_crop_exp_ -= needed; // Or keep accumulated experience, depending on game design - Refactored with Observer Pattern
        cocos2d::log("Level Up! Farming Level is now: %d", player_crop_level_);

        // Can trigger upgrade effects or notify UI update here - Refactored with Observer Pattern
    }
}

void StatisticsManager::OnAnimalAdded() {
    total_animals_owned_++;
}

void StatisticsManager::OnAnimalRemoved() {
    if (total_animals_owned_ > 0) total_animals_owned_--;
}

void StatisticsManager::UpgradeFarmHouse() {
    farm_house_level_++;
}

void StatisticsManager::AddAnimalFood(const std::string& food_type, int amount) {
    animal_food_stock_[food_type] += amount;
}

bool StatisticsManager::ConsumeAnimalFood(const std::string& food_type, int amount) {
    if (animal_food_stock_[food_type] >= amount) {
        animal_food_stock_[food_type] -= amount;
        return true;
    }
    return false;
}

int StatisticsManager::GetFoodCount(const std::string& food_type) {
    if (animal_food_stock_.find(food_type) != animal_food_stock_.end()) {
        return animal_food_stock_[food_type];
    }
    return 0;
}

void StatisticsManager::RecordProduction(const std::string& product_name, int count) {
    production_history_[product_name] += count;
}

} // namespace th_valley