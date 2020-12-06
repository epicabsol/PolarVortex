#pragma once

#include "../game/Camera.h"
#include "../game/Screen.h"
#include "../render/SpriteInstance.h"

class Allocator;
class DynamicCollider;
class Player;
class SpriteAnimation;
class UIAnimation;
class UIDockContainer;
class UISprite;
class World;
class GLMesh;
class GLShaderProgram;

class WorldScreen : public Screen {
private:
    World* _World;
    Camera _MainCamera;
    Player* _Player;
    GLMesh* _TileMesh;
    GLShaderProgram* _TileShaderProgram;
    UIDockContainer* _HUDContainer;
    UIDockContainer* _LeftContainer;
    UIDockContainer* _RightContainer;
    UISprite* _WeaponSprite;
    const GLTexture* _GridTexture;

    virtual void RenderViewportContents(size_t index) override;
    void RebuildTileMesh();

public:
    WorldScreen(Allocator& allocator);
    virtual ~WorldScreen() override;

    inline World* GetWorld() { return this->_World; }

    virtual void Update(float timestep) override;
    virtual void Resize(size_t width, size_t height) override;
};