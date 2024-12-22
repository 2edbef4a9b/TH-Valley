#ifndef CLIENT_CONTROLLER_H_
#define CLIENT_CONTROLLER_H_

#include "boost/asio.hpp"
#include "server/universe_server.h"

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

    void StartUniverseServer();
    void StopUniverseServer();
    void Update();

    ClientState client_state_{ClientState::kStartUp};
    boost::asio::io_context server_io_context_;
    std::thread single_player_thread_;
    UniverseServer universe_server_{server_io_context_, "65472"};
};

}  // namespace th_valley

#endif  // CLIENT_CONTROLLER_H_
