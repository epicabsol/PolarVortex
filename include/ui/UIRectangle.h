#pragma once

#include "../render/Math.h"

struct UIRectangle {
    Vector2 Position;
    Vector2 Size;

    UIRectangle() : Position(), Size() { }
    UIRectangle(Vector2 position, Vector2 size) : Position(position), Size(size) { }
    UIRectangle(float x, float y, float width, float height) : Position(x, y), Size(width, height) { }
};