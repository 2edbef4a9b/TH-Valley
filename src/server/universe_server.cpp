#include "server/universe_server.h"

#include <chrono>

#include "utility/logger.h"

namespace th_valley {

UniverseServer::UniverseServer(boost::asio::io_context& io_context,
                               std::string_view port)
    : Server(io_context, port) {}

void UniverseServer::RunMainLoop() {
    Logger::GetInstance().LogInfo("Universe server running main loop.");
    auto tick_interval = std::chrono::milliseconds(kServerTickInterval);
    while (IsRunning()) {
        auto start_time = std::chrono::steady_clock::now();

        Update();

        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_time = end_time - start_time;
        if (elapsed_time < tick_interval) {
            std::this_thread::sleep_for(tick_interval - elapsed_time);
        }

        // Log if the loop is taking longer than the tick interval
        if (elapsed_time > tick_interval) {
            Logger::GetInstance().LogWarning(
                "Universe server loop took longer than expected: {} ms",
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    elapsed_time)
                    .count());
        }
    }
    Logger::GetInstance().LogInfo("Universe server main loop ended.");
}

void UniverseServer::Update() {
    // Update game state here
    Logger::GetInstance().LogInfo("UniverseServer game loop running.");
}

}  // namespace th_valley
