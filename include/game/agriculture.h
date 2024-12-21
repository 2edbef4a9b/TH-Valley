#ifndef AGRICULTURE_H_
#define AGRICULTURE_H_

#include "game/weather.h"
#include "game/worldtime.h"

class agriculture {
protected:
    WorldTime *CurrentTime;
    Weather *CurrentWeather;
};

#endif  // AGRICULTURE_H_
