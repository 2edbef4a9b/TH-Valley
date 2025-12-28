#include "game/worldtime.h"
#include "i_time_observer.h" // Include interface here to call methods
#include <algorithm>

// Define the global instance
WorldTime GlobalTime;

WorldTime::WorldTime() {
    Year = 1;
    Month = 1;
    Day = 1;
    Hour = 8;
    Minute = 0;
    Second = 0;
    Season = "Spring";
    isContinue = 1;
}

WorldTime& WorldTime::getInstance() {
    return GlobalTime;
}

void WorldTime::AddObserver(ITimeObserver* obs) {
    observers.push_back(obs);
}

void WorldTime::RemoveObserver(ITimeObserver* obs) {
    auto it = std::remove(observers.begin(), observers.end(), obs);
    if (it != observers.end()) {
        observers.erase(it, observers.end());
    }
}

void WorldTime::NotifyTimeChanged() {
    // Notify all listeners that time ticked
    for (auto* obs : observers) {
        obs->OnTimeTick(this);
    }
}

void WorldTime::NotifyDayChanged() {
    for (auto* obs : observers) {
        obs->OnDayChanged(this);
    }
}

void WorldTime::TimeNext() {
    if (!isContinue) return;

    // --- Simulation of time passing ---
    Second++;
    if (Second >= 60) {
        Second = 0;
        Minute++;
        
        // Notify Observers every minute (or however often you want)
        NotifyTimeChanged(); 

        if (Minute >= 60) {
            Minute = 0;
            Hour++;
            if (Hour >= 24) {
                Hour = 0;
                Day++;
                CalculateSeason(); // Update season if needed
                
                // Notify Observers that a new day started
                NotifyDayChanged();
            }
        }
    }
}

void WorldTime::CalculateSeason() {
    // Basic logic example
    if (Month >= 3 && Month <= 5) Season = "Spring";
    else if (Month >= 6 && Month <= 8) Season = "Summer";
    else if (Month >= 9 && Month <= 11) Season = "Autumn";
    else Season = "Winter";
}

// Implement other stubs (TimeStart, TimeStop, etc.) as needed...
void WorldTime::TimeStart() { isContinue = 1; }
void WorldTime::TimeStop() { isContinue = 0; }
void WorldTime::TimeShow() { /* Cocos2d log or UI update */ }
void WorldTime::TimeSet(int nY, int nM, int nD, int nH, int nMin, int nS) {
    Year = nY; Month = nM; Day = nD; Hour = nH; Minute = nMin; Second = nS;
}