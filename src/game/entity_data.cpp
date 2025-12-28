#include "game/entity_data.h"

namespace th_valley {

EntityData::EntityData() {
    // 初始化默认数值
    status_ = {1000, 100, 100, 100, 0, {100, 200, 300}, 1};
    attack_ = {10, 10};
    defense_ = {5, 5};
}

// --- Getter / Setter 实现 ---

void EntityData::SetHealth(double health) { status_.health = health; }
double EntityData::GetHealth() const { return status_.health; }

void EntityData::SetHunger(double hunger) { status_.hunger = hunger; }
double EntityData::GetHunger() const { return status_.hunger; }

void EntityData::SetThirst(double thirst) { status_.thirst = thirst; }
double EntityData::GetThirst() const { return status_.thirst; }

void EntityData::SetEnergy(double energy) { status_.energy = energy; }
double EntityData::GetEnergy() const { return status_.energy; }

void EntityData::SetExperience(double experience) { 
    status_.experience = experience; 
    status_.LevelUp(experience);
}
double EntityData::GetExperience() const { return status_.experience; }

void EntityData::SetPhysicalDamage(double val) { attack_.physical_damage = val; }
double EntityData::GetPhysicalDamage() const { return attack_.physical_damage; }

void EntityData::SetSpellPower(double val) { attack_.spell_power = val; }
double EntityData::GetSpellPower() const { return attack_.spell_power; }

void EntityData::SetPhysicalDefense(double val) { defense_.physical_defense = val; }
double EntityData::GetPhysicalDefense() const { return defense_.physical_defense; }

void EntityData::SetSpellDefense(double val) { defense_.spell_defense = val; }
double EntityData::GetSpellDefense() const { return defense_.spell_defense; }

// 简单的升级逻辑实现
void EntityData::Status::LevelUp(double exp) {
    if (Level < experiencelist.size() && exp >= experiencelist[Level - 1]) {
        Level++;
        health += 100; // 升级回血/加上限
    }
}

} // namespace th_valley