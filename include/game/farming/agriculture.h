#ifndef AGRICULTURE_H_
#define AGRICULTURE_H_

#include "game/weather.h"
#include "game/worldtime.h"

namespace th_valley {

class Agriculture {
protected:
    // 环境引用 (不负责内存管理，由 Manager 注入)
    WorldTime* CurrentTime = nullptr;
    Weather* CurrentWeather = nullptr;

public:
    Agriculture() = default;
    virtual ~Agriculture() = default;

    // 依赖注入：由 Manager 在创建对象后立即调用
    void bindWorldInformation(WorldTime* Time, Weather* weather) {
        CurrentTime = Time;
        CurrentWeather = weather;
    }
};

} // namespace th_valley

#endif  // AGRICULTURE_H_