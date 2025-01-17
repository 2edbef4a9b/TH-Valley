#ifndef WORLDTIME_H_
#define WORLDTIME_H_

#include <string>

#include "cocos2d.h"

class WorldTime {
public:
    int Year, Month, Day, Hour, Minute, Second;
    std::string Season;
    int isContinue;
    WorldTime() {
        Year = 5000;
        Month = 3;
        Day = 1;
        Hour = 8;
        Minute = 0;
        Second = 5;
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

extern WorldTime GlobalTime;

#endif
