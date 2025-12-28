// worldtime.h
#ifndef WORLDTIME_H_
#define WORLDTIME_H_

#include <string>
#include <vector>
#include "cocos2d.h"

// Forward declaration
class ITimeObserver;

class WorldTime {
private:
    int Year, Month, Day, Hour, Minute, Second;
    std::string Season;
    int isContinue;

    // List of observers (Crops, Animals, etc.)
    std::vector<ITimeObserver*> observers;

    WorldTime();

public:
    // Singleton access (if needed, otherwise relying on extern)
    static WorldTime& getInstance();

    // --- Observer Management ---
    void AddObserver(ITimeObserver* obs);
    void RemoveObserver(ITimeObserver* obs);
    void NotifyTimeChanged();
    void NotifyDayChanged();
    // ---------------------------

    void TimeStart();
    void TimeStop();
    void TimeSet(const int NewYear, const int NewMonth, const int NewDay,
                 const int NewHour, const int NewMinute, const int NewSecond);
    void CalculateSeason();
    void TimeNext(); // This is where logic happens
    void TimeShow();

    // Getters
    int getYear() const { return Year; }
    int getMonth() const { return Month; }
    int getDay() const { return Day; }
    int getHour() const { return Hour; }
    std::string getSeason() const { return Season; }

    // Allow the extern instance to be defined
    friend class WorldTimeInstance; 
};

// Global instance declaration
extern WorldTime GlobalTime;

#endif // WORLDTIME_H_