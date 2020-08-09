#include "render/GLRenderer.h"

#include "assets/AssetManager.h"
#include "game/Window.h"
#include "memory/Memory.h"
#include "render/glad.h"
#include "render/glfw3.h"
#include "render/GLVertexAttribute.h"

GLRenderer* Renderer = nullptr;

float SquareMeshVertices[] = {
    0.5f,  0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
};
const size_t SquareMeshVertexCount = 4;

unsigned char SquareMeshIndices[] = {
    0, 1, 3,
    1, 2, 3,
};
const size_t SquareMeshIndexCount = 6;

GLVertexAttribute SpriteVertexAttributes[] = {
    { 3, GL_FLOAT, sizeof(float) }
};
const size_t SpriteVertexAttributeCount = 1;

GLRenderer::GLRenderer() {
    this->_SquareMesh = RootAllocator.New<GLMesh>(SpriteVertexAttributes, SpriteVertexAttributeCount, SquareMeshVertices, SquareMeshVertexCount, SquareMeshIndices, sizeof(SquareMeshIndices[0]), SquareMeshIndexCount);
    this->_SpriteShader = RootAllocator.New<GLShaderProgram>(STRINGHASH("assets/shaders/SpriteVertexShader.glsl"), STRINGHASH("assets/shaders/SpritePixelShader.glsl"));
}

GLRenderer::~GLRenderer() {
    RootAllocator.Delete(this->_SpriteShader);
    RootAllocator.Delete(this->_SquareMesh);
}

void GLRenderer::DrawMesh(GLMesh* mesh, GLShaderProgram* shader) const {
    glUseProgram(shader->GetProgramHandle());
    glBindVertexArray(mesh->GetVertexArrayHandle());
    glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), mesh->GetIndexFormat(), nullptr);
}

void GLRenderer::DrawSprite() const {
    this->DrawMesh(this->_SquareMesh, this->_SpriteShader);
}