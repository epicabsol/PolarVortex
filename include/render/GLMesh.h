#pragma once

#include <stddef.h>

struct GLVertexAttribute;

class GLMesh {
private:
    unsigned int _VertexArrayHandle; // VAO
    unsigned int _VertexBufferHandle; // VBO
    size_t _VertexSize;
    size_t _VertexCount;
    unsigned int _IndexBufferHandle; // EBO
    size_t _IndexSize;
    size_t _IndexCount;
    unsigned int _IndexFormat; // GLenum

public:
    GLMesh(GLVertexAttribute* vertexAttributes, size_t vertexAttributeCount, const void* vertexData, size_t vertexCount, const void* indexData, size_t indexSize, size_t indexCount);
    ~GLMesh();

    inline unsigned int GetVertexArrayHandle() const { return this->_VertexArrayHandle; }
    inline unsigned int GetVertexBufferHandle() const { return this->_VertexBufferHandle; }
    inline size_t GetVertexSize() const { return this->_VertexSize; }
    inline size_t GetVertexCount() const { return this->_VertexCount; }
    inline unsigned int GetIndexBufferHandle() const { return this->_IndexBufferHandle; }
    inline size_t GetIndexSize() const { return this->_IndexSize; }
    inline size_t GetIndexCount() const { return this->_IndexCount; }
    inline unsigned int GetIndexFormat() const { return this->_IndexFormat; }
};