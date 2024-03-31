#include <forest/renderer.h>
#include <rengine/core/graphics.h>

struct REngineRenderer : public Renderer {
    REngineRenderer(REngine::Graphics* gr)
        : gr(gr)
    {}

    REngine::Graphics* gr;
};
