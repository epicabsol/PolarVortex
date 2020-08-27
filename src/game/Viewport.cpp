#include "game/Viewport.h"

#include "game/Camera.h"
#include "ui/UIElement.h"

#define UI_GOAL_HEIGHT 256

void Viewport::SetCamera(Camera* camera) {
    this->_Camera = camera;
    if (this->_Camera != nullptr && this->_Width > 0 && this->_Height > 0) {
        this->_Camera->SetAspectRatio((float)this->_Width / this->_Height);
    }
}

void Viewport::SetRootUIElement(UIElement* element) {
    this->_RootUIElement = element;
    if (element != nullptr) {
        element->LayoutInto(UIRectangle(0.0f, 0.0f, this->_Width / (float)this->_UIScale, this->_Height / (float)this->_UIScale));
    }
}

void Viewport::SetLayout(int x, int y, int width, int height) {
    this->_X = x;
    this->_Y = y;
    this->_Width = width;
    this->_Height = height;
    this->_UIScale = (int)ceil(this->_Height / UI_GOAL_HEIGHT);
    if (this->_Camera != nullptr && this->_Width > 0 && this->_Height > 0) {
        this->_Camera->SetAspectRatio((float)this->_Width / this->_Height);
    }
    if (this->_RootUIElement != nullptr) {
        this->_RootUIElement->LayoutInto(UIRectangle(0.0f, 0.0f, width / (float)this->_UIScale, height / (float)this->_UIScale));
    }
}