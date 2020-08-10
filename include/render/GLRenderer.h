#pragma once

#include "GLMesh.h"
#include "GLShaderProgram.h"
#include "GLTexture.h"
#include "Math.h"

class Viewport;

class GLRenderer {
private:
    GLMesh* _SquareMesh;
    GLShaderProgram* _SpriteShader;
    GLTexture* _DefaultTexture;
    hmm_mat4 _Projection;

public:
    GLRenderer();
    ~GLRenderer();

    void BeginViewport(Viewport* viewport);
    void DrawMesh(GLMesh* mesh, GLShaderProgram* shader) const;
    void DrawSprite(GLTexture* texture, float x, float y, float z, float width, float height) const;
};

extern GLRenderer* Renderer;