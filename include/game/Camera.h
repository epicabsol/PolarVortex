#pragma once

class Allocator;

class Camera {
private:
    float _X;
    float _Y;
    float _Width;
    float _Height;

public:
    inline Camera(Allocator& allocator, float x, float y, float width, float height) : _X(x), _Y(y), _Width(width), _Height(height) { }

    inline float GetX() const { return this->_X; }
    inline float GetY() const { return this->_Y; }
    inline float GetWidth() const { return this->_Width; }
    inline float GetHeight() const { return this->_Height; }
    inline float GetAspectRatio() const { return this->_Width / this->_Height; }
    inline void SetAspectRatio(float ratio) { this->_Width = this->_Height * ratio; }
};