#ifndef ENTITY_DATA_H_
#define ENTITY_DATA_H_
#include<vector>
namespace th_valley {

class EntityData {
public:
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
        Status& operator=(const Status& giv);
        std::vector<double> experiencelist{100, 100, 200, 300,300, 500, 500, 500, 900};
        int Level;
        void LevelUp(double exp);
    };

    const Status casterExist = {1000, 100, 100, 100,0};
    const Status saberExist = {1200, 100, 100, 100,0};
    const Status shilderExist = {2000, 100, 100, 100,0};

    struct Attack {
        double physical_damage;
        double spell_power;
        Attack& operator=(const Attack& giv);
        Attack& operator+(const Attack& giv);

    };

    const Attack casterAttack = {10, 100};
    const Attack saberAttack = {100, 10};
    const Attack shilderAttack = {10, 10};

    struct Defense {
        double physical_defense;
        double spell_defense;
        Defense& operator=(const Defense& giv);
        Defense& operator+(const Defense& giv);
    };

    const Defense casterDefense = {10, 10};
    const Defense saberDefense = {100, 10};
    const Defense shilderDefense = {100, 100};

    Status status_{};
    Attack attack_{};
    Defense defense_{};
};

}  // namespace th_valley

#endif  // ENTITY_DATA_H_
