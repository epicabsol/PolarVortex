#pragma once

#include "GLMesh.h"
#include "GLShaderProgram.h"

class GLRenderer {
private:
    GLMesh* _SquareMesh;
    GLShaderProgram* _SpriteShader;

public:
    GLRenderer();
    ~GLRenderer();

    void DrawMesh(GLMesh* mesh, GLShaderProgram* shader) const;
    void DrawSprite() const;
};

extern GLRenderer* Renderer;