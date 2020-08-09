#pragma once

#include "GLMesh.h"
#include "GLShaderProgram.h"
#include "GLTexture.h"

class GLRenderer {
private:
    GLMesh* _SquareMesh;
    GLShaderProgram* _SpriteShader;
    GLTexture* _DefaultTexture;

public:
    GLRenderer();
    ~GLRenderer();

    void DrawMesh(GLMesh* mesh, GLShaderProgram* shader) const;
    void DrawSprite(GLTexture* texture, float x, float y, float z, float width, float height) const;
};

extern GLRenderer* Renderer;