#ifndef TITLE_SCREEN_H_
#define TITLE_SCREEN_H_

#include <functional>
#include <memory>
#include <string_view>

#include "client/client_controller.h"
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

    static cocos2d::Scene* CreateScene(
        const std::weak_ptr<ClientController>& client_controller);

    CREATE_FUNC(TitleScreen);

private:
    void AddBackground(const std::string_view& background_image);
    void AddButton(const std::string_view& text, float pox_x, float pos_y,
                   const std::function<void()>& callback,
                   const std::string_view& color = "purple");
    void AddLabels();
    void AddTitle();

    constexpr static std::string_view kFontPath =
        "assets/fonts/DFHannotateW5-A.ttf";
    constexpr static std::string_view kBackgroundImagePath =
        "assets/gui/title/koishi.jpg";
    constexpr static std::string_view kTitleText =
        "TH Valley\n~ The Last Remote";

    cocos2d::Size visible_size_;
    cocos2d::Vec2 visible_origin_;
    std::weak_ptr<ClientController> client_controller_;
};

}  // namespace th_valley

#endif  // TITLE_SCREEN_H_
