#pragma once

#include "GLMesh.h"
#include "GLShaderProgram.h"
#include "GLTexture.h"
#include "Math.h"
#include "Sprite.h"
#include "SpriteInstance.h"

class Allocator;
class Viewport;
class SpriteFont;

struct SpriteVertex {
    float X;
    float Y;
    float Z;
    float U;
    float V;
};

class GLRenderer {
private:
    Allocator& _Allocator;
    GLMesh* _SquareMesh;
    GLShaderProgram* _SpriteShader;
    GLTexture* _DefaultTexture;
    Matrix _Projection;

public:
    GLRenderer(Allocator& allocator);
    ~GLRenderer();

    const GLShaderProgram* GetSpriteShader() const { return this->_SpriteShader; }
    const Matrix& GetProjection() const { return this->_Projection; }

    void Initialize();
    void BeginViewport(Viewport* viewport);
    void SetProjection(const Matrix& projection);
    GLMesh* CreateSpriteMesh(Allocator& allocator, const SpriteVertex* vertexData, size_t vertexCount, const void* indexData, size_t indexSize, size_t indexCount);
    void DrawMesh(const GLMesh* mesh, const GLShaderProgram* shader) const;
    inline void DrawSprite(const SpriteInstance& instance, float x, float y, float z, float width, float height) const { this->DrawSprite(instance.GetTexture(), instance.GetCurrentFrame().Sprite.UMin, instance.GetCurrentFrame().Sprite.VMin, instance.GetCurrentFrame().Sprite.USize, instance.GetCurrentFrame().Sprite.VSize, x, y, z, width, height); }
    inline void DrawSprite(const GLTexture* texture, float x, float y, float z, float width, float height) const { this->DrawSprite(texture, 0.0f, 0.0f, 1.0f, 1.0f, x, y, z, width, height); }
    inline void DrawSprite(const Sprite& sprite, float x, float y, float z, float width, float height) const { this->DrawSprite(sprite.Texture, sprite.UMin, sprite.VMin, sprite.USize, sprite.VSize, x, y, z, width, height); }
    void DrawSprite(const GLTexture* texture, float u, float v, float uSize, float vSize, float x, float y, float z, float width, float height) const;
    void DrawString(const SpriteFont* font, const char* string, float x, float y, float z, float scale, float maxWidth);
};
