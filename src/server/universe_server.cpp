#include "server/universe_server.h"

#include <chrono>

#include "utility/logger.h"

namespace th_valley {

UniverseServer::UniverseServer(boost::asio::io_context& io_context,
                               const std::string_view port)
    : Server(io_context, port) {
    SetServerName("Universe Server");
}

void UniverseServer::RunMainLoop() {
    Logger::GetInstance().LogInfo("{}: Main loop starting.", GetServerName());
    const auto tick_interval = std::chrono::milliseconds(kServerTickInterval);
    while (IsRunning()) {
        auto start_time = std::chrono::steady_clock::now();
        Update();
        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_time = end_time - start_time;
        // Sleep for the remaining time in the tick interval.
        if (elapsed_time < tick_interval) {
            std::this_thread::sleep_for(tick_interval - elapsed_time);
        }

        // Log if the loop is taking longer than the tick interval.
        if (elapsed_time > tick_interval) {
            Logger::GetInstance().LogWarning(
                "{}: Main loop iteration took {} ms, longer than expected.",
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    elapsed_time)
                    .count());
        }
    }
    Logger::GetInstance().LogInfo("{}: Main loop ended.", GetServerName());
}

void UniverseServer::Update() {
    // Update game state here.
    Logger::GetInstance().LogInfo("{}: Updating game state.", GetServerName());
    Logger::GetInstance().LogInfo(
        "Koishi is behind you. Koishi is behind you. Koishi is behind you.");
}

}  // namespace th_valley
