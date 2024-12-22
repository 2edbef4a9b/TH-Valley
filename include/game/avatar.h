#ifndef AVATAR_H_
#define AVATAR_H_

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

private:
};

}  // namespace th_valley

#endif  // AVATAR_H_
