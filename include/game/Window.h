#pragma once

class Window {
private:
    void* _Window;
    size_t _ClientWidth;
    size_t _ClientHeight;

public:
    Window(const char* title);
    ~Window();

    inline size_t GetClientWidth() const { return this->_ClientWidth; }
    inline size_t GetClientHeight() const { return this->_ClientHeight; }

    void Run();
    void Resized(size_t width, size_t height);
};

extern Window* MainWindow;