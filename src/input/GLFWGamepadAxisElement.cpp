#include "input/GLFWGamepadAxisElement.h"

#include "render/Math.h"

const char* GLFWGamepadAxisElement::GetName() const {
    return this->_Name;
}

float GLFWGamepadAxisElement::GetValue() const {
    return HMM_CLAMP(this->_GamepadState->axes[this->_AxisID] * this->_Flip, 0.0f, 1.0f);
}