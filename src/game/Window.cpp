#include "game/Window.h"

#include "memory/Memory.h"
#include "render/glad.h"
#include "render/glfw3.h"
#include "render/GLRenderer.h"
#include "render/GLTexture.h"

Window* MainWindow = nullptr;

void WindowFramebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->Resized(width, height);
}

Window::Window(const char* title) : _Window(nullptr), _ClientWidth(800), _ClientHeight(600) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow((int)this->_ClientWidth, (int)this->_ClientHeight, title, nullptr, nullptr); // TODO: Check for success
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    this->_Window = window;

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // TODO: Check for success

    glViewport(0, 0, (int)this->_ClientWidth, (int)this->_ClientHeight);

    glfwSetFramebufferSizeCallback(window, WindowFramebufferSizeCallback);
}

Window::~Window() {
    glfwTerminate();
}

void Window::Run() {
    GLTexture* testTexture = RootAllocator.New<GLTexture>(STRINGHASH("assets/sprites/tile_dirt.png"));
    while (!glfwWindowShouldClose((GLFWwindow*)this->_Window)) {
        // TODO: Get input

        // TODO: Update

        // TODO: Render
        glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Renderer->DrawSprite(testTexture);

        glfwSwapBuffers((GLFWwindow*)this->_Window);
        glfwPollEvents();
    }
}

void Window::Resized(size_t width, size_t height) {
    this->_ClientWidth = width;
    this->_ClientHeight = height;

    glViewport(0, 0, (int)this->_ClientWidth, (int)this->_ClientHeight);
}