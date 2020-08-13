#pragma once

#include "../game/Camera.h"
#include "../game/Screen.h"

class World;

class WorldScreen : public Screen {
private:
    World* _World;
    Camera _MainCamera;

    virtual void RenderViewportContents(size_t index) override;

public:
    WorldScreen();
    virtual ~WorldScreen() override;

    inline World* GetWorld() { return this->_World; }

    virtual void Update(float timestep) override;
    virtual void Resize(size_t width, size_t height) override;
};