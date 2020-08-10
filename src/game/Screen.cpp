#include "game/Screen.h"

#include "render/GLRenderer.h"

Screen* CurrentScreen = nullptr;

void Screen::RenderViewportContents(size_t index) {
    Renderer->BeginViewport(&this->_Viewports[index]);
}

void Screen::Render() {
    for (size_t i = 0; i < MAX_VIEWPORT_COUNT; i++) {
        if (this->_Viewports[i].IsEnabled()) {
            this->RenderViewportContents(i);
        }
    }
}