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

    ClientController(const ClientController& other) = delete;
    ClientController& operator=(const ClientController& other) = delete;
    ClientController(ClientController&& other) = delete;
    ClientController& operator=(ClientController&& other) = delete;

    void SetClientState(ClientState client_state);

    static ClientController& GetInstance();

private:
    ClientController() = default;
    ~ClientController() = default;

    void Update();

    ClientState client_state_{ClientState::kStartUp};
};

}  // namespace th_valley

#endif  // CLIENT_CONTROLLER_H_
