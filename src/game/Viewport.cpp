#include "game/Viewport.h"

#include "game/Camera.h"

void Viewport::SetCamera(Camera* camera) {
    this->_Camera = camera;
    if (this->_Camera != nullptr && this->_Width > 0 && this->_Height > 0) {
        this->_Camera->SetAspectRatio((float)this->_Width / this->_Height);
    }
}

void Viewport::SetLayout(int x, int y, int width, int height) {
    this->_X = x;
    this->_Y = y;
    this->_Width = width;
    this->_Height = height;
    if (this->_Camera != nullptr && this->_Width > 0 && this->_Height > 0) {
        this->_Camera->SetAspectRatio((float)this->_Width / this->_Height);
    }
}