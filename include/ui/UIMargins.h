#pragma once

struct UIMargins {
    float Left;
    float Top;
    float Right;
    float Bottom;

    inline UIMargins() : Left(0.0f), Top(0.0f), Right(0.0f), Bottom(0.0f) { }
    inline UIMargins(float left, float top, float right, float bottom) : Left(left), Top(top), Right(right), Bottom(bottom) { }
};