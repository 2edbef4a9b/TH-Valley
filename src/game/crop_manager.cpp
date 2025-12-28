#include "game/crop_manager.h"

namespace th_valley {

CropManager::CropManager() {
    // 在构造时注册所有支持的作物工厂
    RegisterFactories();
}

CropManager::~CropManager() {
    // 清理作物
    for (auto& pair : crop_map_) {
        delete pair.second;
    }
    crop_map_.clear();

    // 清理工厂
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
  // 注册所有在 crop_factory.h 中定义的具体工厂
  // 这里要注意 Key 值（种子名）必须和 PlantCrop 传入的参数一致
  factory_registry_["Strawberry"] = new StrawberryFactory();
  factory_registry_["Strawberry Seeds"] = factory_registry_["Strawberry"]; // 兼容别名

  factory_registry_["Potato"] = new PotatoFactory();
  factory_registry_["Potato Seeds"] = factory_registry_["Potato"];

  factory_registry_["Carrot"] = new CarrotFactory();
  factory_registry_["Carrot Seeds"] = factory_registry_["Carrot"];

  factory_registry_["Turnip"] = new TurnipFactory();
  factory_registry_["Turnip Seeds"] = factory_registry_["Turnip"];
}

bool CropManager::PlantCrop(const Position& pos, const std::string& crop_name) {
    // 1. 检查该位置是否已经有作物
    if (crop_map_.find(pos) != crop_map_.end()) {
        cocos2d::log("Tile occupied!");
        return false;
    }

    // 2. 查找对应的工厂
    auto it = factory_registry_.find(crop_name);
    if (it == factory_registry_.end()) {
        cocos2d::log("No factory found for crop: %s", crop_name.c_str());
        return false;
    }

    // 3. 使用工厂创建作物 (UML中的 Spawn(Factory) 逻辑体现)
    CropFactory* factory = it->second;
    Crops* newCrop = factory->CreateCrop(); // 调用 Factory Method

    // 4. 初始化作物的通用数据 (注入位置、环境依赖)
    newCrop->position = pos;
    newCrop->bindWorldInformation(time_system_, weather_system_);
    
    // 5. 创建 Sprite 并添加到地图 (视图逻辑)
    if (parent_layer_) {
        // 假设 SpecificCrop 构造函数里还没创建 Sprite，或者这里统一根据名字创建
        // 这里演示简单的创建逻辑
        std::string spritePath = "crops/" + newCrop->getCropName() + "_stage0.png";
        newCrop->CropSprite = cocos2d::Sprite::create(spritePath);
        if (newCrop->CropSprite) {
            // 设置坐标转换 (逻辑坐标 -> 像素坐标)
            // 假设每个格子 32x32
            newCrop->CropSprite->setPosition(pos.x * 32 + 16, pos.y * 32 + 16);
            parent_layer_->addChild(newCrop->CropSprite);
        }
    }

    // 6. 放入容器并注册观察者
    crop_map_[pos] = newCrop;
    
    // 自动让时间系统监听这个新作物
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

        // 移除观察者
        if (time_system_) time_system_->RemoveObserver(crop);
        
        // 移除显示对象
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
    // 可以在这里处理渲染排序等逻辑
}

} // namespace th_valley