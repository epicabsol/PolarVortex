#pragma once

#include "../game/Camera.h"
#include "../game/Screen.h"
#include "../render/SpriteInstance.h"

class Allocator;
class DynamicCollider;
class SpriteAnimation;
class UIAnimation;
class UIDockContainer;
class UISprite;
class World;

class WorldScreen : public Screen {
private:
    World* _World;
    Camera _MainCamera;
    DynamicCollider* _Player;
    SpriteAnimation* _PlayerIdleAnimation;
    SpriteInstance _PlayerAnimation;
    UIDockContainer* _HUDContainer;
    UIDockContainer* _RightContainer;
    UISprite* _WeaponSprite;

    virtual void RenderViewportContents(size_t index) override;

public:
    WorldScreen(Allocator& allocator);
    virtual ~WorldScreen() override;

    inline World* GetWorld() { return this->_World; }

    virtual void Update(float timestep) override;
    virtual void Resize(size_t width, size_t height) override;
};