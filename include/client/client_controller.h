#ifndef CLIENT_CONTROLLER_H_
#define CLIENT_CONTROLLER_H_

namespace th_valley {

class ClientController {
public:
    enum class ClientState {
        kLoading,
        kMultiPlayer,
        kQuit,
        kSettings,
        kShutDown,
        kSinglePlayer,
        kStartUp,
        kTitleScreen,
    };

    void SetClientState(ClientState client_state);

private:
    void Update();

    ClientState client_state_{ClientState::kStartUp};
};

}  // namespace th_valley

#endif  // CLIENT_CONTROLLER_H_
