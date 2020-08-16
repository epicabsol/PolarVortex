#pragma once

#include "../render/glfw3.h"

#include "InputElement.h"

/**
 * @brief An input element that reads the state of a button from the given GLFW gamepad state.
 */
class GLFWGamepadButtonElement : public InputElement {
private:
    int _ButtonID;
    const GLFWgamepadstate* _GamepadState;
    const char* _Name;

public:
    inline GLFWGamepadButtonElement(Allocator& allocator, int buttonID, const GLFWgamepadstate* gamepadState, Hash spriteHash, const char* name) : InputElement(allocator, spriteHash), _ButtonID(buttonID), _GamepadState(gamepadState), _Name(name) { }

    virtual const char* GetName() const override;
    virtual float GetValue() const override;
};