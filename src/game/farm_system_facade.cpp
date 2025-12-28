#include "game/farm_system_facade.h"
// 假设这里引入了背包单例，用于扣除种子或添加收获物
#include "game/bag.h" 

namespace th_valley {

FarmSystemFacade::FarmSystemFacade() {
    // 构造函数不进行复杂初始化
}

FarmSystemFacade::~FarmSystemFacade() {
    if (crop_manager_) delete crop_manager_;
    if (animal_manager_) delete animal_manager_;
    if (statistics_manager_) delete statistics_manager_;
    // time_system_ 和 weather_system_ 通常是全局单例的引用，不在此 delete
}

void FarmSystemFacade::InitSystem(cocos2d::Node* map_root_node) {
    // 1. 获取全局环境引用
    time_system_ = &GlobalTime;
    weather_system_ = &GlobalWeather;

    // 2. 初始化统计管理器
    statistics_manager_ = new StatisticsManager();

    // 3. 初始化农作物管理器
    crop_manager_ = new CropManager();
    // 注入渲染层和环境
    crop_manager_->Init(map_root_node, time_system_, weather_system_);

    // 4. 初始化动物管理器
    animal_manager_ = new AnimalManager();
    animal_manager_->Init(map_root_node, time_system_, weather_system_);
    
    cocos2d::log("Farm System Initialized");
}

void FarmSystemFacade::UpdateGame(float dt) {
    // 1. 时间流逝
    if (time_system_) time_system_->TimeNext(); // 内部会 NotifyObserver

    // 2. 天气更新 (可选)
    if (weather_system_) weather_system_->Update(dt);

    // 3. 管理器逻辑更新 (如动物移动插值、作物渲染排序)
    if (crop_manager_) crop_manager_->Update(dt);
    if (animal_manager_) animal_manager_->Update(dt);
}

void FarmSystemFacade::OnInteract(const Position& grid_pos, const cocos2d::Vec2& world_pos, const std::string& selected_tool) {
    // 优先检查动物交互（通常动物在作物之上，且不基于网格）
    // 如果工具是刷子、饲料或者空手，可能是在和动物交互
    bool interact_with_animal = (selected_tool == "Brush" || selected_tool == "Fodder" || selected_tool == "None");
    
    if (interact_with_animal) {
        if (animal_manager_->InteractWithAnimal(world_pos, selected_tool)) {
            // 如果成功交互了动物，就不要继续交互作物了
            return; 
        }
    }

    // 否则进行农作物/土地交互
    HandleCropInteraction(grid_pos, selected_tool);
}

void FarmSystemFacade::HandleCropInteraction(const Position& pos, const std::string& tool) {
    if (!crop_manager_) return;

    Crops* crop = crop_manager_->GetCropAt(pos);

    // 1. 种植逻辑
    if (tool.find("Seeds") != std::string::npos) { // 简单判断工具名是否包含 Seeds
        // 从工具名提取种子类型，例如 "Strawberry Seeds" -> "Strawberry"
        // 这里简化处理，假设 Bag 传递的是作物名
        std::string crop_name = tool; // 需要根据实际字符串处理逻辑修改
        if (tool == "Strawberry Seeds") crop_name = "Strawberry";
        else if (tool == "Potato Seeds") crop_name = "Potato";

        if (PlantCrop(pos, crop_name)) {
            // 种植成功，扣除背包物品 (伪代码)
            Bag::GetInstance()->RemoveItem(tool, 1);
        }
        return;
    }

    // 2. 浇水逻辑
    if (tool == "WateringCan" && crop) {
        crop->CropWatering();
        return;
    }

    // 3. 收获/清除逻辑 (镰刀或空手)
    if ((tool == "Sickle" || tool == "None") && crop) {
        // 假设 Crops 类有个 CanHarvest() 方法
        if (crop->CurrentGrowthStage >= crop->MaxGrowthStage) {
            // --- 收获 ---
            int yield = 1; // 应该从 crop 获取 output count
            std::string product = crop->getCropName(); // 或者 productItemName
            
            // 移除作物 (如果是草莓这种多季作物，可能只是重置阶段)
            // 这里简单处理为移除
            crop_manager_->RemoveCrop(pos); 
            
            // 更新统计
            if (statistics_manager_) {
                statistics_manager_->OnCropHarvested(10); // +10 经验
                statistics_manager_->RecordProduction(product, yield);
            }

            // 添加到背包 (伪代码)
            Bag::GetInstance()->AddItem(product, yield);
            
            cocos2d::log("Harvested %s x%d", product.c_str(), yield);
        } else if (tool == "Sickle" && crop->isDeath) {
            // --- 清除枯死作物 ---
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
    // 伪代码：序列化所有管理器数据
    // json j;
    // j["time"] = time_system_->Save();
    // j["crops"] = crop_manager_->Save();
    // j["animals"] = animal_manager_->Save();
    // j["stats"] = statistics_manager_->Save();
    // FileUtils::writeStringToFile(j.dump(), save_file);
}

void FarmSystemFacade::LoadData(const std::string& save_file) {
    // 反序列化逻辑
}

} // namespace th_valley