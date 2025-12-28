// i_time_observer.h
#ifndef I_TIME_OBSERVER_H_
#define I_TIME_OBSERVER_H_

class WorldTime;

class ITimeObserver {
public:
    virtual ~ITimeObserver() = default;
    virtual void OnTimeTick(WorldTime* time) = 0;
    virtual void OnDayChanged(WorldTime* time) = 0;
};

#endif // I_TIME_OBSERVER_H_