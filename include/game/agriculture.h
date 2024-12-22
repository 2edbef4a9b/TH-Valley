#ifndef AGRICULTURE_H_
#define AGRICULTURE_H_

#include "game/weather.h"
#include "game/worldtime.h"

class agriculture {
protected:
    WorldTime* CurrentTime;
    Weather* CurrentWeather;

public:
    void getWorldInformation(WorldTime* Time, Weather* weather) {
        CurrentTime = Time;
        CurrentWeather = weather;
    }
};

#endif  // AGRICULTURE_H_
