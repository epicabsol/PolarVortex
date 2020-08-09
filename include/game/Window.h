#pragma once

class Window {
private:
    void* _Window;
    size_t _ClientWidth;
    size_t _ClientHeight;

public:
    Window();
    ~Window();

    void Run();
    void Resized(size_t width, size_t height);
};

extern Window* MainWindow;