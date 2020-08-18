#include "game/Screen.h"

#include "game/PolarVortexGame.h"
#include "render/GLRenderer.h"
#include "ui/UIElement.h"

void Screen::RenderViewportContents(size_t index) {
    Game->GetRenderer().BeginViewport(&this->_Viewports[index]);
}

void Screen::Render() {
    for (size_t i = 0; i < MAX_VIEWPORT_COUNT; i++) {
        if (this->_Viewports[i].IsEnabled()) {
            this->RenderViewportContents(i);
            UIElement* rootElement = this->_Viewports[i].GetRootUIElement();
            if (rootElement != nullptr) {
                Game->GetRenderer().SetProjection(Math_Orthographic(0.0f, this->_Viewports[i].GetWidth() / (float)this->_Viewports[i].GetUIScale(), this->_Viewports[i].GetHeight() / (float)this->_Viewports[i].GetUIScale(), 0.0f, -100.0f, 100.0f));
                rootElement->Render(UIRectangle(0.0f, 0.0f, this->_Viewports[i].GetWidth() / (float)this->_Viewports[i].GetUIScale(), this->_Viewports[i].GetHeight() / (float)this->_Viewports[i].GetUIScale()));
            }
        }
    }
}