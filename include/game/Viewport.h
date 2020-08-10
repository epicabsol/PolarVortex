#pragma once

class Camera;

/**
 * @brief Defines a rectangular region of the screen that can render a `Camera`'s point of view.
 */
class Viewport {
private:
    Camera* _Camera;
    int _X;
    int _Y;
    int _Width;
    int _Height;

public:
    /**
     * @brief Constructs a disabled, zero-size Viewport.
     */
    inline Viewport() : _Camera(nullptr), _X(0), _Y(0), _Width(0), _Height(0) { }

    inline Camera* GetCamera() const { return this->_Camera; }
    inline int GetX() const { return this->_X; }
    inline int GetY() const { return this->_Y; }
    inline int GetWidth() const { return this->_Width; }
    inline int GetHeight() const { return this->_Height; }
    inline bool IsEnabled() const { return this->_Camera != nullptr; }

    void SetCamera(Camera* camera);
    void SetLayout(int x, int y, int width, int height);
};