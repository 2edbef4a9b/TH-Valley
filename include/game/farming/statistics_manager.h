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

    // --- 农作物统计 (替代 CropProduction) ---
    void OnCropPlanted();               // 种植时调用
    void OnCropHarvested(int exp_gain); // 收获时调用，增加经验

    int GetPlayerCropLevel() const { return player_crop_level_; }
    int GetTotalCropsHarvested() const { return total_crops_harvested_; }
    int GetTotalPlanted() const { return total_planted_; }
    double GetCurrentCropExp() const { return current_crop_exp_; }

    // --- 动物/农舍统计 (替代 FarmHouse) ---
    void OnAnimalAdded();
    void OnAnimalRemoved();
    
    // 农舍等级
    int GetFarmHouseLevel() const { return farm_house_level_; }
    void UpgradeFarmHouse(); // 升级农舍

    // 饲料库存管理 (原 FarmHouse::FoodLeft)
    // 这里只做简单计数，如果逻辑复杂建议移入 Bag 或 Silo 类
    void AddAnimalFood(const std::string& food_type, int amount);
    bool ConsumeAnimalFood(const std::string& food_type, int amount);
    int GetFoodCount(const std::string& food_type);

    // 产出记录 (原 FarmHouse::Output)
    void RecordProduction(const std::string& product_name, int count);

    // --- 数据持久化接口 ---
    void Save(json& j);
    void Load(const json& j);

private:
    // --- 农业数据 ---
    int player_crop_level_;
    double current_crop_exp_;
    std::vector<double> crop_level_up_exp_; // 升级所需经验表
    
    int total_crops_harvested_;
    int total_planted_;

    // --- 畜牧业数据 ---
    int farm_house_level_;
    int total_animals_owned_;
    
    // 饲料库存: <FoodName, Count>
    std::map<std::string, int> animal_food_stock_;
    
    // 产出历史记录: <ProductName, TotalCount>
    std::map<std::string, int> production_history_;

    // 内部逻辑
    void CheckCropLevelUp();
};

} // namespace th_valley

#endif // STATISTICS_MANAGER_H_