#ifndef WEATHER_H_
#define WEATHER_H_

#include "cocos2d.h"
#include "worldtime.h"
#include <string>

class Weather {
public:
    const int WeatherTotal = 3 ; // the total types of weather
    WorldTime CurrentTime;
    std::string WeatherType;
    int TypeDuringTime;
    int Temperature;
    Weather() {
        WeatherType = "Clear";
        TypeDuringTime = 0;
    }
    void WeatherSet(const std::string weather);
    void WeatherShow();
    void WeatherAutomaticUpdate();
};

#endif
