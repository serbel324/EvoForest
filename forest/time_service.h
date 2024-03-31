#pragma once

#include <functional>

#include <memory>
#include <vector>

class TimedEvent;

class TimeService {
public:
    using Callback = std::function<void()>;

public:
    void PlanEvent(double fuseSec, Callback&& callback);
    void Tick(double elapsedSec);

private:
    std::vector<std::unique_ptr<TimedEvent>> _timedEvents;
};

class TimedEvent {
public:
    TimedEvent(double fuseSec, TimeService::Callback&& callback);
    bool Tick(double elapsedSec);

private:
    double _fuseSec;
    TimeService::Callback _callback;
};
