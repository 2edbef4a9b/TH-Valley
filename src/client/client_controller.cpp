#include "client/client_controller.h"

#include "cocos2d.h"
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
    switch (client_state_) {
        case ClientState::kLoading:
            Logger::GetInstance().LogInfo("GameState Change to Loading");
            break;
        case ClientState::kMultiPlayer:
            Logger::GetInstance().LogInfo("GameState Change to MultiPlayer");
            break;
        case ClientState::kQuit:
            Logger::GetInstance().LogInfo("GameState Change to Quit");
            break;
        case ClientState::kSettings:
            Logger::GetInstance().LogInfo("GameState Change to Settings");
            break;
        case ClientState::kShutDown:
            Logger::GetInstance().LogInfo("GameState Change to ShutDown");
            break;
        case ClientState::kSinglePlayer:
            Logger::GetInstance().LogInfo("GameState Change to SinglePlayer");
            break;
        case ClientState::kStartUp:
            SetClientState(ClientState::kTitleScreen);
            Logger::GetInstance().LogInfo("GameState Change to StartUp");
            break;
        case ClientState::kTitleScreen:
            cocos2d::Director::getInstance()->replaceScene(
                TitleScreen::create());
            Logger::GetInstance().LogInfo("GameState Change to TitleScreen");
            break;
        default:
            break;
    }
}

}  // namespace th_valley
