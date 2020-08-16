#pragma once

#include "../render/glfw3.h"

#include "InputElement.h"

class GLFWGamepadAxisElement : public InputElement {
private:
    int _AxisID;
    const GLFWgamepadstate* _GamepadState;
    const char* _Name;
    float _Flip;

public:
    inline GLFWGamepadAxisElement(Allocator& allocator, int axisID, const GLFWgamepadstate* gamepadState, Hash spriteHash, const char* name, float flip) : InputElement(allocator, spriteHash), _AxisID(axisID), _GamepadState(gamepadState), _Name(name), _Flip(flip) { }

    virtual const char* GetName() const override;
    virtual float GetValue() const override;
};