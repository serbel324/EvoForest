#pragma once
#include <vector>
#include <string>

#include <rengine/core/frame.h>
#include <memory>
#include <deque>

namespace REngine {

class Driver {
public:
    using UPtr = std::unique_ptr<Driver>;

public:
    static Driver* King();
    static void Promote(UPtr&& newKing);

    virtual ~Driver() = default;

    virtual void Initialize() = 0;
    virtual void Run() = 0;

    template <class Derived>
    Derived* Cast() {
        return dynamic_cast<Derived*>(this);
    }
};


class SingleFrameDriver : public Driver {
public:
    struct Settings {
        float minimumUpdateDelayMs;
    };

public:
    SingleFrameDriver(Frame::UPtr&& frame, Settings settings = {});

    void Initialize() override;
    void Run() override;

private:
    Frame::UPtr _frame;
    const Settings _settings;
};

} // namespace REngine
