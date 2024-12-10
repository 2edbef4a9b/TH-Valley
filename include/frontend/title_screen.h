#ifndef TITLE_SCREEN_H_
#define TITLE_SCREEN_H_

#include <string_view>

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

private:
    void AddBackground(const std::string_view& background_image);
    void AddButton(const std::string_view& text, float pox_x, float pos_y);
    void AddLabels();
    void AddTitle();
    cocos2d::Size visible_size_;
    cocos2d::Vec2 visible_origin_;
};

}  // namespace th_valley

#endif  // TITLE_SCREEN_H_
