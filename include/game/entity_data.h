#ifndef ENTITY_DATA_H_
#define ENTITY_DATA_H_

#include <vector>

namespace th_valley {

class EntityData {
public:
    EntityData(); // 添加构造函数初始化

    void SetHealth(double health);
    void SetHunger(double hunger);
    void SetThirst(double thirst);
    void SetEnergy(double energy);
    void SetExperience(double experience);

    [[nodiscard]] double GetHealth() const;
    [[nodiscard]] double GetHunger() const;
    [[nodiscard]] double GetThirst() const;
    [[nodiscard]] double GetEnergy() const;
    [[nodiscard]] double GetExperience() const;

    void SetPhysicalDamage(double physical_damage);
    void SetSpellPower(double spell_power);
    [[nodiscard]] double GetPhysicalDamage() const;
    [[nodiscard]] double GetSpellPower() const;

    void SetPhysicalDefense(double physical_defense);
    void SetSpellDefense(double spell_defense);
    [[nodiscard]] double GetPhysicalDefense() const;
    [[nodiscard]] double GetSpellDefense() const;

private:
    struct Status {
        double health;
        double hunger;
        double thirst;
        double energy;
        double experience;
        std::vector<double> experiencelist;
        int Level;
        
        void LevelUp(double exp);
    };

    struct Attack {
        double physical_damage;
        double spell_power;
    };

    struct Defense {
        double physical_defense;
        double spell_defense;
    };

    Status status_{};
    Attack attack_{};
    Defense defense_{};
};

}  // namespace th_valley

#endif  // ENTITY_DATA_H_