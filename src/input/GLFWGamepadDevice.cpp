#include "input/GLFWGamepadDevice.h"

#include "input/GLFWGamepadButtonElement.h"
#include "input/GLFWGamepadAxisElement.h"
#include "memory/Memory.h"
#include "render/glfw3.h"
#include "render/GLTexture.h"

struct GamepadElementInfo {
    const char* Name;
    const Hash SpriteHash;
};

struct GamepadAxisInfo : GamepadElementInfo {
    int Axis;
    float Flip;
};

// Must match the indices defined as GLFW_GAMEPAD_BUTTON_...
const GamepadElementInfo GamepadButtons[] = {
    { "A Button", STRINGHASH("assets/sprites/controls/xbox/btn_a.png") },
    { "B Button", STRINGHASH("assets/sprites/controls/xbox/btn_b.png") },
    { "X Button", STRINGHASH("assets/sprites/controls/xbox/btn_x.png") },
    { "Y Button", STRINGHASH("assets/sprites/controls/xbox/btn_y.png") },
    { "Left Bumper", STRINGHASH("assets/sprites/controls/xbox/btn_lb.png") },
    { "Right Bumper", STRINGHASH("assets/sprites/controls/xbox/btn_rb.png") },
    { "Back Button", STRINGHASH("assets/sprites/controls/xbox/btn_back.png") },
    { "Start Button", STRINGHASH("assets/sprites/controls/xbox/btn_start.png") },
    { "Guide Button", STRINGHASH("assets/sprites/controls/xbox/btn_start.png") }, // TODO: Should we draw a guide button sprite?
    { "Left Joystick", STRINGHASH("assets/sprites/controls/xbox/btn_ls.png") },
    { "Right Joystick", STRINGHASH("assets/sprites/controls/xbox/btn_rs.png") },
    { "Up Button", STRINGHASH("assets/sprites/controls/xbox/dpad_up.png") },
    { "Right Button", STRINGHASH("assets/sprites/controls/xbox/dpad_right.png") },
    { "Down Button", STRINGHASH("assets/sprites/controls/xbox/dpad_down.png") },
    { "Left Button", STRINGHASH("assets/sprites/controls/xbox/dpad_left.png") }
};

const GamepadAxisInfo GamepadAxes[] = {
    { "Left Joystick Left", STRINGHASH("assets/sprites/controls/xbox/lstick_left.png"), GLFW_GAMEPAD_AXIS_LEFT_X, -1.0f },
    { "Left Joystick Right", STRINGHASH("assets/sprites/controls/xbox/lstick_right.png"), GLFW_GAMEPAD_AXIS_LEFT_X, 1.0f },
    { "Left Joystick Up", STRINGHASH("assets/sprites/controls/xbox/lstick_up.png"), GLFW_GAMEPAD_AXIS_LEFT_Y, -1.0f },
    { "Left Joystick Down", STRINGHASH("assets/sprites/controls/xbox/lstick_down.png"), GLFW_GAMEPAD_AXIS_LEFT_Y, 1.0f },
    { "Right Joystick Left", STRINGHASH("assets/sprites/controls/xbox/rstick_left.png"), GLFW_GAMEPAD_AXIS_RIGHT_X, -1.0f },
    { "Right Joystick Right", STRINGHASH("assets/sprites/controls/xbox/rstick_right.png"), GLFW_GAMEPAD_AXIS_RIGHT_X, 1.0f },
    { "Right Joystick Up", STRINGHASH("assets/sprites/controls/xbox/rstick_up.png"), GLFW_GAMEPAD_AXIS_RIGHT_Y, -1.0f },
    { "Right Joystick Down", STRINGHASH("assets/sprites/controls/xbox/rstick_down.png"), GLFW_GAMEPAD_AXIS_RIGHT_Y, 1.0f },
    { "Left Trigger", STRINGHASH("assets/sprites/controls/xbox/trigger_l.png"), GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, 1.0f },
    { "Right Trigger", STRINGHASH("assets/sprites/controls/xbox/trigger_r.png"), GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, 1.0f },
};

GLFWGamepadDevice::GLFWGamepadDevice(Allocator& allocator, int joystickID) : _Allocator(allocator), _JoystickID(joystickID), _State(), _Sprite(RootAllocator.New<GLTexture>(STRINGHASH("assets/sprites/controls/xbox/controller.png"))) {
    for (size_t i = 0; i < GLFW_GAMEPAD_BUTTON_COUNT; i++) {
        this->_InputElements[i] = RootAllocator.New<GLFWGamepadButtonElement>((int)i, &this->_State, GamepadButtons[i].SpriteHash, GamepadButtons[i].Name);
    }
    for (size_t i = 0; i < GLFW_GAMEPAD_AXIS_COUNT; i++) {
        this->_InputElements[i + GLFW_GAMEPAD_BUTTON_COUNT] = RootAllocator.New<GLFWGamepadAxisElement>(GamepadAxes[i].Axis, &this->_State, GamepadAxes[i].SpriteHash, GamepadAxes[i].Name, GamepadAxes[i].Flip);
    }
}

GLTexture* GLFWGamepadDevice::GetSprite() const {
    return this->_Sprite;
}

const char* GLFWGamepadDevice::GetName() const {
    return glfwGetGamepadName(this->_JoystickID);
}

bool GLFWGamepadDevice::IsConnected() const {
    return this->_IsConnected;
}

const InputElement* const* GLFWGamepadDevice::GetInputElements() const {
    return &this->_InputElements[0];
}

size_t GLFWGamepadDevice::GetInputElementCount() const {
    return sizeof(this->_InputElements) / sizeof(this->_InputElements[0]);
}

void GLFWGamepadDevice::Update() {
    this->_IsConnected = glfwGetGamepadState(this->_JoystickID, &this->_State) == GLFW_TRUE;
}