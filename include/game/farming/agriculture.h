#ifndef AGRICULTURE_H_
#define AGRICULTURE_H_

#include "game/weather.h"
#include "game/worldtime.h"

namespace th_valley {

class Agriculture {
protected:
    // Environment reference (doesn't manage memory, injected by Manager) - Refactored with Factory Pattern
    WorldTime* CurrentTime = nullptr;
    Weather* CurrentWeather = nullptr;

public:
    Agriculture() = default;
    virtual ~Agriculture() = default;

    // Dependency injection: called by Manager immediately after object creation - Refactored with Factory Pattern
    void bindWorldInformation(WorldTime* Time, Weather* weather) {
        CurrentTime = Time;
        CurrentWeather = weather;
    }
};

} // namespace th_valley

#endif  // AGRICULTURE_H_