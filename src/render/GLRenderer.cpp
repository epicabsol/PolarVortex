#include "render/GLRenderer.h"

#include "assets/AssetManager.h"
#include "game/Window.h"
#include "memory/Memory.h"
#include "render/glad.h"
#include "render/glfw3.h"
#include "render/GLTexture.h"
#include "render/GLVertexAttribute.h"

GLRenderer* Renderer = nullptr;

float SquareMeshVertices[] = {
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
};
const size_t SquareMeshVertexCount = 4;

unsigned char SquareMeshIndices[] = {
    0, 1, 3,
    1, 2, 3,
};
const size_t SquareMeshIndexCount = 6;

GLVertexAttribute SpriteVertexAttributes[] = {
    { 3, GL_FLOAT, sizeof(float) },
    { 2, GL_FLOAT, sizeof(float) },
};
const size_t SpriteVertexAttributeCount = 2;

unsigned char DefaultTextureData[] = {
    255, 0, 255, 255, 255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255,
    255, 0, 255, 255, 255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255,
    0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255, 255, 0, 255, 255,
    0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255, 255, 0, 255, 255,
};
unsigned int DefaultTextureWidth = 4;
unsigned int DefaultTextureHeight = 4;

GLRenderer::GLRenderer() {
    this->_SquareMesh = RootAllocator.New<GLMesh>(SpriteVertexAttributes, SpriteVertexAttributeCount, SquareMeshVertices, SquareMeshVertexCount, SquareMeshIndices, sizeof(SquareMeshIndices[0]), SquareMeshIndexCount);
    this->_SpriteShader = RootAllocator.New<GLShaderProgram>(STRINGHASH("assets/shaders/SpriteVertexShader.glsl"), STRINGHASH("assets/shaders/SpritePixelShader.glsl"));
    this->_SpriteShader->SetTextureIndex("Texture", 0);
    this->_DefaultTexture = RootAllocator.New<GLTexture>(DefaultTextureWidth, DefaultTextureHeight, DefaultTextureData);
}

GLRenderer::~GLRenderer() {
    RootAllocator.Delete(this->_DefaultTexture);
    RootAllocator.Delete(this->_SpriteShader);
    RootAllocator.Delete(this->_SquareMesh);
}

void GLRenderer::DrawMesh(GLMesh* mesh, GLShaderProgram* shader) const {
    //glUseProgram(shader->GetProgramHandle());
    glBindVertexArray(mesh->GetVertexArrayHandle());
    glDrawElements(GL_TRIANGLES, (int)mesh->GetIndexCount(), (int)mesh->GetIndexFormat(), nullptr);
}

void GLRenderer::DrawSprite(GLTexture* texture) const {
    glUseProgram(this->_SpriteShader->GetProgramHandle());

    if (texture == nullptr) {
        texture = this->_DefaultTexture;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetHandle());
    this->DrawMesh(this->_SquareMesh, this->_SpriteShader);
}