#ifndef WORLDTIME_H_
#define WORLDTIME_H_

#include "cocos2d.h"
#include <string>

class WorldTime {
public:
    int Year, Month, Day, Hour, Minute, Second;
    std::string Season;
    int isContinue;
    WorldTime() {
        Year = 0;
        Month = 0;
        Day = 0;
        Hour = 0;
        Minute = 0;
        Second = 0;
        Season = "Spring";
        isContinue = 1;
    }
    void TimeStart();
    void TimeStop();
    void TimeSet(const int NewYear, const int NewMonth, const int NewDay,
                 const int NewHour, const int NewMinute, const int NewSecond);
    void CalculateSeason();
    void TimeNext();
    void TimeShow();
};

#endif
