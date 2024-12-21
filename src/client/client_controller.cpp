#include "client/client_controller.h"

#include "cocos2d.h"
#include "frontend/game_scene.h"
#include "frontend/title_screen.h"
#include "utility/logger.h"

namespace th_valley {

void ClientController::SetClientState(const ClientState client_state) {
    client_state_ = client_state;
    Update();
}

ClientController& ClientController::GetInstance() {
    static ClientController instance;
    return instance;
}

void ClientController::Update() {
    auto director = cocos2d::Director::getInstance();
    switch (client_state_) {
        case ClientState::kLoading:
            Logger::GetInstance().LogInfo("GameState Change to Loading");
            break;
        case ClientState::kMultiPlayer:
            Logger::GetInstance().LogInfo("GameState Change to MultiPlayer");
            break;
        case ClientState::kQuit:
            Logger::GetInstance().LogInfo("GameState Change to Quit");
            director->end();
            break;
        case ClientState::kSettings:
            Logger::GetInstance().LogInfo("GameState Change to Settings");
            break;
        case ClientState::kShutDown:
            Logger::GetInstance().LogInfo("GameState Change to ShutDown");
            break;
        case ClientState::kSinglePlayer:
            Logger::GetInstance().LogInfo("GameState Change to SinglePlayer");
            director->getOpenGLView()->setDesignResolutionSize(
                384, 216, ResolutionPolicy::NO_BORDER);
            director->replaceScene(GameScene::create());
            break;
        case ClientState::kStartUp:
            Logger::GetInstance().LogInfo("GameState Change to StartUp");
            SetClientState(ClientState::kTitleScreen);
            break;
        case ClientState::kTitleScreen:
            Logger::GetInstance().LogInfo("GameState Change to TitleScreen");
            director->replaceScene(TitleScreen::create());
            break;
        default:
            break;
    }
}

}  // namespace th_valley
