#include <forest/time_service.h>

void TimeService::PlanEvent(double fuseSec, Callback&& callback) {
    _timedEvents.emplace_back(new TimedEvent(fuseSec, std::forward<Callback>(callback)));
}

void TimeService::Tick(double elapsedSec) {
    for (auto it = _timedEvents.begin(); it != _timedEvents.end(); ++it) {
        if ((*it)->Tick(elapsedSec)) {
            it->reset();
        }
    }

    _timedEvents.erase(
        std::remove_if(
            _timedEvents.begin(),
            _timedEvents.end(),
            [](const std::unique_ptr<TimedEvent>& p) { return !p; }
        ),
        _timedEvents.end()
    );
}

TimedEvent::TimedEvent(double fuseSec, TimeService::Callback&& callback)
    : _fuseSec(fuseSec)
    , _callback(callback)
{}

bool TimedEvent::Tick(double elapsedSec) {
    _fuseSec -= elapsedSec;
    if (_fuseSec <= 0) {
        _callback();
        return true;
    }
    return false;
}