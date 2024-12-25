#include "server/universe_server.h"

#include <chrono>

#include "game/animals.h"
#include "game/crop_production.h"
#include "game/crops.h"
#include "game/farm_house.h"
#include "game/worldtime.h"
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
    GlobalTime.TimeNext();

    GlobalWeather.WeatherAutomaticUpdate(GlobalTime);

    GlobalCropProduction.getWorldInformation(&GlobalTime, &GlobalWeather);
    for (int CropCount = 0; CropCount < GlobalCropProduction.AllCrops.size();
         CropCount++) {
        if (GlobalCropProduction.AllCrops[CropCount] != nullptr)
            GlobalCropProduction.AllCrops[CropCount]->CropAutomaticUpdate();
    }

    for (int AnimalCount = 0; AnimalCount < GlobalFarmHouse.AllAnimals.size();
         AnimalCount++) {
        GlobalFarmHouse.AllAnimals[AnimalCount]->AnimalAutomaticUpdate();
    }

    // Logger::GetInstance().LogInfo("{}: Updating game state.",
    // GetServerName());
}

}  // namespace th_valley
