#ifndef AVATAR_H_
#define AVATAR_H_

#include <string_view>
#include <vector>

#include "2d/CCActionInterval.h"
#include "2d/CCAnimation.h"
#include "game/entity.h"

namespace th_valley {

class Avatar : public Entity {
public:
    Avatar() = default;
    ~Avatar() override = default;

    Avatar(const Avatar& other) = delete;
    Avatar& operator=(const Avatar& other) = delete;
    Avatar(Avatar&& other) = delete;
    Avatar& operator=(Avatar&& other) = delete;

    bool init() override;
    void update(float delta) override;

    void UseTool(std::string_view tool);
    void UseFishingRod(cocos2d::Vec2 tarpos, double kScale);
    void UseWateringCan(cocos2d::Vec2 tarpos, Direction direction,
                        double kScale);
    void UseOtherTools(std::string_view tool, cocos2d::Vec2 tarpos,
                       Direction direction, double kScale);
    //void Attacking(std::string weaponTypes);
    ////void LevelUpShow();

    //void MeleeAttack(cocos2d::Vec2 tarpos,Direction direction);
    //void RangedAttack(cocos2d::Vec2 tarpos,Direction direction);
    void RenderMove();
    void ChangeDirection(Direction direction) override;

private:
    struct AvatarTexture {
        cocos2d::Texture2D* left;
        cocos2d::Texture2D* right;
        cocos2d::Texture2D* up;
        cocos2d::Texture2D* down;
    };

    void InitAnimation(std::string_view avatar_name);
    void InitTexture(std::string_view avatar_name);
    AvatarTexture avatar_texture_{};
    std::vector<cocos2d::Animate*> move_animations_;

    std::vector<std::string> ocupationlist{"caster", "saber", "shilder"};
    std::vector<std::string> weponlist{"wand", "sword", "shield"};

    std::string profession;
    std::string weapon;
};

}  // namespace th_valley

#endif  // AVATAR_H_
