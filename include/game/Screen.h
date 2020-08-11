#pragma once

#include "Viewport.h"
#include "../memory/StackAllocator.h"

#define MAX_VIEWPORT_COUNT 4

class Screen {
protected:
    Viewport _Viewports[MAX_VIEWPORT_COUNT];
    virtual void RenderViewportContents(size_t index);

public:
    Screen() { }
    virtual ~Screen() { }

    virtual void Update(float timestep) = 0;
    void Render();
    virtual void Resize(size_t width, size_t height) = 0;
};
