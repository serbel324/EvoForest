#pragma once
#include <memory>

struct Renderer {
    using SPtr = std::shared_ptr<Renderer>;

    template<class Impl>
    Impl* As() {
        return dynamic_cast<Impl*>(this);
    }

    virtual ~Renderer() = default;
};
