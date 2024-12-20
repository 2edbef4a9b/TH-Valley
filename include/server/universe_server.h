#ifndef UNIVERSE_SERVER_H_
#define UNIVERSE_SERVER_H_

#include "server/server.h"

namespace th_valley {

class UniverseServer final : public Server {
public:
    UniverseServer(boost::asio::io_context& io_context, std::string_view port);
    ~UniverseServer() override = default;

    UniverseServer(const UniverseServer& other) = delete;
    UniverseServer& operator=(const UniverseServer& other) = delete;
    UniverseServer(UniverseServer&& other) = delete;
    UniverseServer& operator=(UniverseServer&& other) = delete;

protected:
    void RunMainLoop() override;

private:
    void Update();
};

}  // namespace th_valley

#endif  // UNIVERSE_SERVER_H_
