#ifndef WORLDTIME_H_
#define WORLDTIME_H_

#include "cocos2d.h"
#include <string>

class WorldTime {
public:
    int Year, Month, Day, Hour, Minute, Second;
    std::string Season;
    void TimeStart();
    void TimeStop();
    void TimeSet(const int NewYear, const int NewMonth, const int NewDay,
                 const int NewHour, const int NewMinute, const int NewSecond);
    void CalculateSeason();
};

#endif
