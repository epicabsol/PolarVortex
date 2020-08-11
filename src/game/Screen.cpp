#include "game/Screen.h"

#include "render/GLRenderer.h"
#include "game/PolarVortexGame.h"

void Screen::RenderViewportContents(size_t index) {
    Game->GetRenderer().BeginViewport(&this->_Viewports[index]);
}

void Screen::Render() {
    for (size_t i = 0; i < MAX_VIEWPORT_COUNT; i++) {
        if (this->_Viewports[i].IsEnabled()) {
            this->RenderViewportContents(i);
        }
    }
}