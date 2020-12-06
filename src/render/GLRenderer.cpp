#include "render/GLRenderer.h"

#include "assets/AssetManager.h"
#include "game/Window.h"
#include "game/Viewport.h"
#include "game/Camera.h"
#include "memory/Memory.h"
#include "render/glad.h"
#include "render/glfw3.h"
#include "render/GLTexture.h"
#include "render/GLVertexAttribute.h"
#include "render/Math.h"
#include "render/SpriteFont.h"

static float SquareMeshVertices[] = {
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
};
static const size_t SquareMeshVertexCount = 4;

static unsigned char SquareMeshIndices[] = {
    0, 1, 3,
    1, 2, 3,
};
static const size_t SquareMeshIndexCount = 6;

static GLVertexAttribute SpriteVertexAttributes[] = {
    { 3, GL_FLOAT, sizeof(float) },
    { 2, GL_FLOAT, sizeof(float) },
};
static const size_t SpriteVertexAttributeCount = 2;

static unsigned char DefaultTextureData[] = {
    255, 0, 255, 255, 255, 0, 255, 255, 0, 255, 0, 255, 0, 255, 0, 255,
    255, 0, 255, 255, 255, 0, 255, 255, 0, 255, 0, 255, 0, 255, 0, 255,
    0, 255, 0, 255, 0, 255, 0, 255, 255, 0, 255, 255, 255, 0, 255, 255,
    0, 255, 0, 255, 0, 255, 0, 255, 255, 0, 255, 255, 255, 0, 255, 255,
};
static unsigned int DefaultTextureWidth = 4;
static unsigned int DefaultTextureHeight = 4;

GLRenderer::GLRenderer(Allocator& allocator) : _Allocator(allocator) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLRenderer::~GLRenderer() {
    this->_Allocator.Delete(this->_DefaultTexture);
    this->_Allocator.Delete(this->_SpriteShader);
    this->_Allocator.Delete(this->_SquareMesh);
}

void GLRenderer::Initialize() {
    this->_SquareMesh = this->_Allocator.New<GLMesh>(SpriteVertexAttributes, SpriteVertexAttributeCount, SquareMeshVertices, SquareMeshVertexCount, SquareMeshIndices, sizeof(SquareMeshIndices[0]), SquareMeshIndexCount);
    this->_SpriteShader = this->_Allocator.New<GLShaderProgram>(STRINGHASH("assets/shaders/SpriteVertexShader.glsl"), STRINGHASH("assets/shaders/SpritePixelShader.glsl"));
    this->_SpriteShader->SetTextureIndex("Texture", 0);
    this->_DefaultTexture = this->_Allocator.New<GLTexture>(DefaultTextureWidth, DefaultTextureHeight, DefaultTextureData);
}

void GLRenderer::BeginViewport(Viewport* viewport) {
    glViewport(viewport->GetX(), viewport->GetY(), viewport->GetWidth(), viewport->GetHeight());
    glClear(GL_COLOR_BUFFER_BIT);

    Camera* camera = viewport->GetCamera();
    if (camera != nullptr) {
        this->_Projection = Math_Orthographic(camera->GetPosition().X - camera->GetSize().X * 0.5f, camera->GetPosition().X + camera->GetSize().X * 0.5f, camera->GetPosition().Y - camera->GetSize().Y * 0.5f, camera->GetPosition().Y + camera->GetSize().Y * 0.5f, -100.0f, 100.0f);
    }
}

void GLRenderer::SetProjection(const Matrix& projection) {
    this->_Projection = projection;
}

GLMesh* GLRenderer::CreateSpriteMesh(Allocator& allocator, const SpriteVertex* vertexData, size_t vertexCount, const void* indexData, size_t indexSize, size_t indexCount) {
    return allocator.New<GLMesh>(SpriteVertexAttributes, SpriteVertexAttributeCount, vertexData, vertexCount, indexData, indexSize, indexCount);
}

void GLRenderer::DrawMesh(const GLMesh* mesh, const GLShaderProgram* shader) const {
    glUseProgram(shader->GetProgramHandle());
    glBindVertexArray(mesh->GetVertexArrayHandle());
    glDrawElements(GL_TRIANGLES, (int)mesh->GetIndexCount(), (int)mesh->GetIndexFormat(), nullptr);
}

void GLRenderer::DrawSprite(const GLTexture* texture, float u, float v, float uSize, float vSize, float x, float y, float z, float width, float height) const {
    glUseProgram(this->_SpriteShader->GetProgramHandle());

    Matrix transform = Math_Translate(Math_Vec3(x, y, z)) * Math_Scale(Math_Vec3(width, height, 1.0f));
    unsigned int transformUniform = glGetUniformLocation(this->_SpriteShader->GetProgramHandle(), "Transform");
    glUniformMatrix4fv(transformUniform, 1, GL_FALSE, &transform.Elements[0][0]);
    unsigned int projectionUniform = glGetUniformLocation(this->_SpriteShader->GetProgramHandle(), "Projection");
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &this->_Projection.Elements[0][0]);
    unsigned int uvPositionUniform = glGetUniformLocation(this->_SpriteShader->GetProgramHandle(), "UVPosition");
    glUniform2f(uvPositionUniform, u, v);
    unsigned int uvSizeUniform = glGetUniformLocation(this->_SpriteShader->GetProgramHandle(), "UVSize");
    glUniform2f(uvSizeUniform, uSize, vSize);

    if (texture == nullptr) {
        texture = this->_DefaultTexture;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetHandle());
    this->DrawMesh(this->_SquareMesh, this->_SpriteShader);
}

void GLRenderer::DrawString(const SpriteFont* font, const char* string, float x, float y, float z, float scale, float maxWidth) {
    float startX = x;

    // Move down from the caps-height to the first baseline
    y -= (font->GetLineHeight() - font->GetDescent()) * scale;

    size_t length = strlen(string);
    for (size_t i = 0; i < length; i++) {
        const SpriteFontGlyph* glyph = font->GetGlyph(string[i]);
        if (glyph == nullptr) {
            printf("WARNING: Missing glyph in font!\n");
            continue;
        }
        // Check if we need to wrap this character to the next line.
        if (maxWidth > 0.0f && x + (glyph->Left + glyph->Sprite.USize * glyph->Sprite.Texture->GetWidth()) * scale > maxWidth + startX) {
            x = startX;
            y -= font->GetLineHeight() * scale;
        }

        this->DrawSprite(glyph->Sprite, x + (glyph->Left + glyph->Sprite.USize * glyph->Sprite.Texture->GetWidth() * 0.5f) * scale, y + (glyph->Top - glyph->Sprite.VSize * glyph->Sprite.Texture->GetHeight() * 0.5f) * scale, z, glyph->Sprite.USize * glyph->Sprite.Texture->GetWidth() * scale, glyph->Sprite.VSize * glyph->Sprite.Texture->GetHeight() * scale);

        x += glyph->Advance * scale;
    }
}