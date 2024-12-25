#include "game/entity_data.h"
#include "utility/logger.h"

namespace th_valley {

void EntityData::SetHealth(double health) { status_.health = health; }

void EntityData::SetHunger(double hunger) { status_.hunger = hunger; }

void EntityData::SetThirst(double thirst) { status_.thirst = thirst; }

void EntityData::SetEnergy(double energy) { status_.energy = energy; }

void EntityData::SetExperience(double experience) {
    status_.experience = experience;
}

double EntityData::GetHealth() const { return status_.health; }

double EntityData::GetHunger() const { return status_.hunger; }

double EntityData::GetThirst() const { return status_.thirst; }

double EntityData::GetEnergy() const { return status_.energy; }

double EntityData::GetExperience() const { return status_.experience; }

void EntityData::SetPhysicalDamage(double physical_damage) {
    attack_.physical_damage = physical_damage;
}

void EntityData::SetSpellPower(double spell_power) {
    attack_.spell_power = spell_power;
}

double EntityData::GetPhysicalDamage() const { return attack_.physical_damage; }

double EntityData::GetSpellPower() const { return attack_.spell_power; }

void EntityData::SetPhysicalDefense(double physical_defense) {
    defense_.physical_defense = physical_defense;
}

void EntityData::SetSpellDefense(double spell_defense) {
    defense_.spell_defense = spell_defense;
}

double EntityData::GetPhysicalDefense() const {
    return defense_.physical_defense;
}

double EntityData::GetSpellDefense() const { return defense_.spell_defense; }

EntityData::Attack& EntityData::Attack::operator=(
    const EntityData::Attack& giv) {
    if (&giv != this) {
        this->physical_damage = giv.physical_damage;
        this->spell_power = giv.spell_power;
    }
    return *this;
}

EntityData::Attack& EntityData::Attack::operator+(
    const EntityData::Attack& giv) {
    this->physical_damage += giv.physical_damage;
    this->spell_power += giv.spell_power;
    return *this;
}

EntityData::Defense& EntityData::Defense::operator=(
    const EntityData::Defense& giv) {
    if (&giv != this) {
        this->physical_defense = giv.physical_defense;
        this->spell_defense = giv.spell_defense;
    }
    return *this;
}

EntityData::Defense& EntityData::Defense::operator+(
    const EntityData::Defense& giv) {
    this->physical_defense += giv.physical_defense;
    this->spell_defense += giv.spell_defense;
    return *this;
}

EntityData::Status& EntityData::Status::operator=(
    const EntityData::Status& giv) {
    if (&giv != this) {
        this->health = giv.health;
        this->hunger = giv.hunger;
        this->thirst = giv.thirst;
        this->energy = giv.energy;
        this->experience = giv.experience;
    }
    return *this;
}


void EntityData::Status::LevelUp(double exp) {
    if (Level < 10) {
        experience += exp;
        if (experience >=experiencelist[Level-1]){
            Level++;
            if (Level < 10) {
                experience -= experiencelist[Level - 1];
            } else {
                Logger::GetInstance().LogInfo("can not get exp");
            }
        }
    } else {
        Logger::GetInstance().LogInfo("Level Cap Reached");
    }
}

}  // namespace th_valley
