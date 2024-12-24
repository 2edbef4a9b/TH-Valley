#include "game/weather.h"

#include <random>
#include <string>

#include "game/worldtime.h"

void Weather::WeatherAutomaticUpdate(WorldTime Time) {
    // TimeUpdate
    WorldTime LastTime = CurrentTime;
    CurrentTime = Time;

    // Rainy Update
    const int Duration = 60 * 60 * 24 * 1;
    std::string LastType = WeatherType;
    if (WeatherType != "Rainy") {
        std::random_device rd;
        std::mt19937 gen(rd());
        int MaxRandom;
        if (CurrentTime.Season == "Spring") MaxRandom = 1500;
        if (CurrentTime.Season == "Summer") MaxRandom = 1000;
        if (CurrentTime.Season == "Autumn") MaxRandom = 1500;
        if (CurrentTime.Season == "Winter") MaxRandom = 5000;
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

    // Temperature Update
    if (CurrentTime.Hour != LastTime.Hour) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 5);
        int DeltaTemperature = dist(gen);
        int BasicTemperature;
        if (CurrentTime.Season == "Spring") BasicTemperature = 15;
        if (CurrentTime.Season == "Summer") BasicTemperature = 25;
        if (CurrentTime.Season == "Autumn") BasicTemperature = 15;
        if (CurrentTime.Season == "Winter") BasicTemperature = 5;
        if (CurrentTime.Hour <= 6 || CurrentTime.Hour >= 19) {
            BasicTemperature -= DeltaTemperature;
        } else if (CurrentTime.Hour >= 10 && CurrentTime.Hour <= 15) {
            BasicTemperature += DeltaTemperature;
        } else
            Temperature = BasicTemperature;
    }
}

void Weather::WeatherSet(std::string weather) { WeatherType = weather; }

void Weather::WeatherShow() {
    CCLOG("%s Duration: %d\n Temperature: %d\n", WeatherType.c_str(),
          TypeDuringTime, Temperature);
}
