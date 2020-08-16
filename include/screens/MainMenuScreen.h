#pragma once

#include "../game/Screen.h"

class Allocator;
class GLTexture;
class Camera;

class MainMenuScreen : public Screen {
private:
    GLTexture* _TestTexture;
    Camera* _MainCamera;

    virtual void RenderViewportContents(size_t index) override;

public:
    MainMenuScreen(Allocator& allocator);
    virtual ~MainMenuScreen() override;

    virtual void Update(float timestep) override;
    virtual void Resize(size_t width, size_t height) override;
};