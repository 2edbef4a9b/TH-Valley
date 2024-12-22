#ifndef AVATAR_H_
#define AVATAR_H_

#include <string_view>

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

    void InitEntity(cocos2d::Node* parent) override;
    void ChangeDirection(Direction direction) override;

private:
    struct AvatarTexture {
        cocos2d::Texture2D* left;
        cocos2d::Texture2D* right;
        cocos2d::Texture2D* up;
        cocos2d::Texture2D* down;
    };

    void InitTexture(std::string_view avatar_name);
    AvatarTexture avatar_texture_;
};

}  // namespace th_valley

#endif  // AVATAR_H_
