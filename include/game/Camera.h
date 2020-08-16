#pragma once

#include "../render/Math.h"

class Allocator;

class Camera {
private:
    Vector2 _Position;
    Vector2 _Size;

public:
    inline Camera(Allocator& allocator, float x, float y, float width, float height) : _Position(x, y), _Size(width, height) { }

    inline const Vector2& GetPosition() const { return this->_Position; }
    inline void SetPosition(const Vector2& position) { this->_Position = position; }
    inline const Vector2& GetSize() const { return this->_Size; }
    inline void SetSize(const Vector2& size) { this->_Size = size; }
    inline float GetAspectRatio() const { return this->_Size.X / this->_Size.Y; }
    inline void SetAspectRatio(float ratio) { this->_Size.X = this->_Size.Y * ratio; }
};