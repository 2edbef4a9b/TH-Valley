#include "game/worldtime.h"

void WorldTime::TimeNext() {
    // Second, Minute, Hour Update
    Second++;
    if (Second == 60) {
        Second = 0;
        Minute++;
    }
    if (Minute == 60) {
        Minute = 0;
        Hour++;
    }
    if (Hour == 24) {
        Hour = 0;
        Day++;
    }

    // Month Update
    if (Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 ||
        Month == 10 || Month == 12) {
        if (Day == 32) {
            Day = 1;
            Month++;
        }
    } else if (Month == 2) {
        if (Day == 29) {
            Day = 1;
            Month++;
        }
    } else {
        if (Day == 31) {
            Day = 1;
            Month++;
        }
    }

    // Year Update
    if (Month == 13) {
        Month = 1;
        Year++;
    }

    // SeasonUpdate
    CalculateSeason();
}

void WorldTime::CalculateSeason() {
    if (Month >= 3 && Month <= 5)
        Season = "Spring";
    else if (Month >= 6 && Month <= 8)
        Season = "Summer";
    else if (Month >= 9 && Month <= 11)
        Season = "Autumn";
    else
        Season = "Winter";
}

void WorldTime::TimeStart() { isContinue = 1; }

void WorldTime::TimeStop() { isContinue = 0; }

void WorldTime::TimeSet(const int NewYear, const int NewMonth, const int NewDay,
                        const int NewHour, const int NewMinute,
                        const int NewSecond) {
    Year = NewYear;
    Month = NewMonth;
    Day = NewDay;
    Hour = NewHour;
    Minute = NewMinute;
    Second = NewSecond;
}

void WorldTime::TimeShow() {
    CCLOG("%d %d %d %d %d %d\n", Year, Month, Day, Hour, Minute, Second);
    CCLOG("%s\n", Season.c_str());
}
