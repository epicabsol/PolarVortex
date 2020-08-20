#pragma once

#include "InputDevice.h"
#include "../render/glfw3.h"

#define GLFW_GAMEPAD_BUTTON_COUNT (GLFW_GAMEPAD_BUTTON_LAST + 1)
#define GLFW_GAMEPAD_AXIS_COUNT (10)

class Allocator;

class GLFWGamepadDevice : public InputDevice {
private:
    Allocator& _Allocator;
    int _JoystickID;
    GLFWgamepadstate _State;
    const GLTexture* _Sprite;
    InputElement* _InputElements[GLFW_GAMEPAD_BUTTON_COUNT + GLFW_GAMEPAD_AXIS_COUNT];
    bool _IsConnected;

public:
    GLFWGamepadDevice(Allocator& allocator, int joystickID);

    virtual const GLTexture* GetSprite() const override;
    virtual const char* GetName() const override;
    virtual bool IsConnected() const override;
    virtual const InputElement* const* GetInputElements() const override;
    virtual size_t GetInputElementCount() const override;

    void Update();
};