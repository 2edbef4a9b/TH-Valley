#ifndef WEATHER_H_
#define WEARHER_H_

#include "cocos2d.h"
#include "worldtime.h"
#include <string>

class Weather {
public:
    const int WeatherTotal; // the total types of weather
    WorldTime CurrentTime;
    std::string WeatherType;
    int Temperature;
    void WeatherSet(const std::string weather);
    void WeatherShow();
};

#endif
