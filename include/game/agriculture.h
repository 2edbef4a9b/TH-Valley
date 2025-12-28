#ifndef AGRICULTURE_H_
#define AGRICULTURE_H_

#include "game/weather.h"
#include "game/worldtime.h"

class Agriculture {
protected:
    // 改为 nullptr，不负责创建，只负责引用
    WorldTime* CurrentTime = nullptr;
    Weather* CurrentWeather = nullptr;

public:
    Agriculture() = default;
    virtual ~Agriculture() = default;

    // 由外部管理器（如 FarmManager 或 Map）调用此函数注入全局的时间和天气对象
    void bindWorldInformation(WorldTime* Time, Weather* weather) {
        CurrentTime = Time;
        CurrentWeather = weather;
    }
    
    // 之前的 getWorldInformation 命名有点像 Getter，但实际是 Setter，建议改名为 set 或 bind
};

#endif  // AGRICULTURE_H_