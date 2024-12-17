#include "client/client_controller.h"

#include "CCPlatformMacros.h"
#include "cocos2d.h"
#include "frontend/title_screen.h"

th_valley::ClientController& th_valley::ClientController::GetInstance() {
    static ClientController instance;
    return instance;
}

void th_valley::ClientController::SetClientState(
    const ClientState client_state) {
    client_state_ = client_state;
    Update();
}

void th_valley::ClientController::Update() {
    switch (client_state_) {
        case ClientState::kLoading:
            CCLOG("GameState Change to Loading");
            break;
        case ClientState::kMultiPlayer:
            CCLOG("GameState Change to MultiPlayer");
            break;
        case ClientState::kQuit:
            CCLOG("GameState Change to Quit");
            break;
        case ClientState::kSettings:
            CCLOG("GameState Change to Settings");
            break;
        case ClientState::kShutDown:
            CCLOG("GameState Change to ShutDown");
            break;
        case ClientState::kSinglePlayer:
            CCLOG("GameState Change to SinglePlayer");
            break;
        case ClientState::kStartUp:
            SetClientState(ClientState::kTitleScreen);
            CCLOG("GameState Change to StartUp");
            break;
        case ClientState::kTitleScreen:
            cocos2d::Director::getInstance()->replaceScene(
                th_valley::TitleScreen::create());
            CCLOG("GameState Change to TitleScreen");
            break;
        default:
            break;
    }
}
