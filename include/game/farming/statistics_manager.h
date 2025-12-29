#ifndef STATISTICS_MANAGER_H_
#define STATISTICS_MANAGER_H_

#include <string>
#include <map>
#include <vector>
#include "cocos2d.h"

namespace th_valley {

class StatisticsManager {
public:
    StatisticsManager();
    ~StatisticsManager() = default;

    // --- Crop Statistics (replaces CropProduction) - Refactored with Observer Pattern ---
    void OnCropPlanted();               // Called when planting - Refactored with Observer Pattern
    void OnCropHarvested(int exp_gain); // Called when harvesting, increases experience - Refactored with Observer Pattern

    int GetPlayerCropLevel() const { return player_crop_level_; }
    int GetTotalCropsHarvested() const { return total_crops_harvested_; }
    int GetTotalPlanted() const { return total_planted_; }
    double GetCurrentCropExp() const { return current_crop_exp_; }

    // --- Animal/Farmhouse Statistics (replaces FarmHouse) - Refactored with Observer Pattern ---
    void OnAnimalAdded();
    void OnAnimalRemoved();

    // Farmhouse level - Refactored with Observer Pattern
    int GetFarmHouseLevel() const { return farm_house_level_; }
    void UpgradeFarmHouse(); // Upgrade farmhouse - Refactored with Observer Pattern

    // Feed inventory management (original FarmHouse::FoodLeft) - Refactored with Observer Pattern
    // Simple counting here, if logic is complex suggest moving to Bag or Silo class - Refactored with Observer Pattern
    void AddAnimalFood(const std::string& food_type, int amount);
    bool ConsumeAnimalFood(const std::string& food_type, int amount);
    int GetFoodCount(const std::string& food_type);

    // Production record (original FarmHouse::Output) - Refactored with Observer Pattern
    void RecordProduction(const std::string& product_name, int count);

    // --- Data Persistence Interface - Refactored with Observer Pattern ---
    void Save(json& j);
    void Load(const json& j);

private:
    // --- Agricultural Data - Refactored with Observer Pattern ---
    int player_crop_level_;
    double current_crop_exp_;
    std::vector<double> crop_level_up_exp_; // Experience table for upgrades - Refactored with Observer Pattern

    int total_crops_harvested_;
    int total_planted_;

    // --- Livestock Data - Refactored with Observer Pattern ---
    int farm_house_level_;
    int total_animals_owned_;

    // Feed inventory: <FoodName, Count> - Refactored with Observer Pattern
    std::map<std::string, int> animal_food_stock_;

    // Production history: <ProductName, TotalCount> - Refactored with Observer Pattern
    std::map<std::string, int> production_history_;

    // Internal logic - Refactored with Observer Pattern
    void CheckCropLevelUp();
};

} // namespace th_valley

#endif // STATISTICS_MANAGER_H_