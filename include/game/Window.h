#pragma once

#include <stddef.h>

class Allocator;
class GLFWGamepadDevice;
class InputDevice;

#define WINDOW_GAMEPAD_COUNT (4)

class Window {
private:
    void* _Window;
    size_t _ClientWidth;
    size_t _ClientHeight;
    InputDevice* _InputDevices[WINDOW_GAMEPAD_COUNT];
    GLFWGamepadDevice* _Gamepads[WINDOW_GAMEPAD_COUNT];

public:
    Window(Allocator& allocator, const char* title);
    ~Window();

    inline size_t GetClientWidth() const { return this->_ClientWidth; }
    inline size_t GetClientHeight() const { return this->_ClientHeight; }
    inline InputDevice** GetInputDevices() { return &this->_InputDevices[0]; }
    inline InputDevice* GetInputDevice(size_t index) { return this->_InputDevices[index]; }
    inline size_t GetInputDeviceCount() { return sizeof(this->_InputDevices) / sizeof(this->_InputDevices[0]); }

    void Run();
    void Resized(size_t width, size_t height);
};
