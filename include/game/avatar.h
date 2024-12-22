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
};

}  // namespace th_valley

#endif  // AVATAR_H_