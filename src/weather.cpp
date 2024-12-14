#include "weather.h"
#include "worldtime.h"
#include <random>
#include <string>

void Weather::WeatherAutomaticUpdate() {
    // Rainy Update
    const int Duration = 100;
    std::string LastType = WeatherType;
    if (WeatherType != "Rainy") {
        std::random_device rd;
        std::mt19937 gen(rd());
        int MaxRandom;
        if (CurrentTime.Season == "Spring") MaxRandom = 20;
        if (CurrentTime.Season == "Summer") MaxRandom = 10;
        if (CurrentTime.Season == "Autumn") MaxRandom = 15;
        if (CurrentTime.Season == "Winter") MaxRandom = 50;
        std::uniform_int_distribution<> dist(1, MaxRandom);
        int RandomNumber = dist(gen);
        if (RandomNumber == 1) {
            if (Temperature < 0)
                WeatherType = "Snowy";
            else
                WeatherType = "Rainy";
        }
    }

    // Clear Update
    if (WeatherType != "Clear") {
        std::random_device rd;
        std::mt19937 gen(rd());
        int MaxRandom;
        MaxRandom = std::max(1, Duration - TypeDuringTime);
        std::uniform_int_distribution<> dist(1, MaxRandom);
        int RandomNumber = dist(gen);
        if (RandomNumber == 1) WeatherType = "Clear";
    }

    // Duration Update
    if (WeatherType == LastType) TypeDuringTime++;
}

void Weather::WeatherSet(std::string weather) { WeatherType = weather; }

void Weather::WeatherShow() { CCLOG("%s", WeatherType); }
