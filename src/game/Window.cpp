#include "game/Window.h"

#include "render/glad.h"
#include "game/Screen.h"
#include "input/GLFWGamepadDevice.h"
#include "memory/Memory.h"
#include "render/glfw3.h"
#include "render/GLRenderer.h"
#include "render/GLTexture.h"

#include "game/PolarVortexGame.h"

static void WindowFramebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->Resized(width, height);
}

static void GLFWErrorCallback(int code, const char* message) {
    printf("[ERROR]: GLFW error %d (%s)\n", code, message);
}

Window::Window(Allocator& allocator, const char* title) : _Window(nullptr), _ClientWidth(0), _ClientHeight(0) {
    glfwSetErrorCallback(GLFWErrorCallback);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, title, nullptr, nullptr); // TODO: Check for success
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    this->_Window = window;

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // TODO: Check for success

    glfwSetFramebufferSizeCallback(window, WindowFramebufferSizeCallback);

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    this->_ClientWidth = width;
    this->_ClientHeight = height;
}

Window::~Window() {
    glfwTerminate();
}

void Window::Initialize() {
    for (size_t i = 0; i < WINDOW_GAMEPAD_COUNT; i++) {
        this->_Gamepads[i] = RootAllocator.New<GLFWGamepadDevice>((int)i);
        this->_Gamepads[i]->Update();
        this->_InputDevices[i] = this->_Gamepads[i];
    }
}

void Window::Run() {
    while (!glfwWindowShouldClose((GLFWwindow*)this->_Window)) {
        for (size_t i = 0; i < WINDOW_GAMEPAD_COUNT; i++) {
            this->_Gamepads[i]->Update();
        }

        Game->Tick();

        glfwSwapBuffers((GLFWwindow*)this->_Window);
        glfwPollEvents();
    }
}

void Window::Resized(size_t width, size_t height) {
    this->_ClientWidth = width;
    this->_ClientHeight = height;

    Screen* currentScreen = Game->GetCurrentScreen();
    if (currentScreen != nullptr) {
        currentScreen->Resize(width, height);
    }
}