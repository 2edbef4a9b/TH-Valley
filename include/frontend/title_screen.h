#ifndef TITLE_SCREEN_H_
#define TITLE_SCREEN_H_

#include "cocos2d.h"

namespace th_valley {

class TitleScreen : public cocos2d::Scene {
public:
    TitleScreen() = default;
    ~TitleScreen() override = default;
    TitleScreen(const TitleScreen& other) = delete;
    TitleScreen& operator=(const TitleScreen& other) = delete;
    TitleScreen(TitleScreen&& other) = delete;
    TitleScreen& operator=(TitleScreen&& other) = delete;

    bool init() override;

    CREATE_FUNC(TitleScreen);
};

}  // namespace th_valley

#endif  // TITLE_SCREEN_H_
