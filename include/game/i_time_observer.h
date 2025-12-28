// i_time_observer.h
#ifndef I_TIME_OBSERVER_H_
#define I_TIME_OBSERVER_H_

// Forward declaration to avoid circular dependency
class WorldTime;

class ITimeObserver {
public:
    virtual ~ITimeObserver() = default;

    // Called every game tick/minute
    virtual void OnTimeTick(WorldTime* time) = 0;

    // Called when the day changes
    virtual void OnDayChanged(WorldTime* time) = 0;
};

#endif // I_TIME_OBSERVER_H_