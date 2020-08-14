#include "input/GLFWGamepadButtonElement.h"

const char* GLFWGamepadButtonElement::GetName() const {
    return this->_Name;
}

float GLFWGamepadButtonElement::GetValue() const {
    return this->_GamepadState->buttons[this->_ButtonID] ? 1.0f : 0.0f;
}