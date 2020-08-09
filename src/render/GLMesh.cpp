#include "render/GLMesh.h"

#include "render/glad.h"
#include "render/GLVertexAttribute.h"

GLMesh::GLMesh(GLVertexAttribute* vertexAttributes, size_t vertexAttributeCount, const void* vertexData, size_t vertexCount, const void* indexData, size_t indexSize, size_t indexCount) : _VertexArrayHandle(0), _VertexBufferHandle(0), _VertexSize(0), _VertexCount(vertexCount), _IndexBufferHandle(0), _IndexSize(indexSize), _IndexCount(indexCount), _IndexFormat(GL_UNSIGNED_BYTE) {
    // Create the VAO
    glGenVertexArrays(1, &this->_VertexArrayHandle);
    glBindVertexArray(this->_VertexArrayHandle);

    // Determine the vertex size
    for (size_t i = 0; i < vertexAttributeCount; i++) {
        this->_VertexSize += vertexAttributes[i].ComponentCount * vertexAttributes[i].ComponentSize;
    }

    // Create the vertex buffer and upload the vertices
    glGenBuffers(1, &this->_VertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, this->_VertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, this->_VertexSize * this->_VertexCount, vertexData, GL_STATIC_DRAW);

    // Set up vertex attributes
    size_t offset = 0;
    for (size_t i = 0; i < vertexAttributeCount; i++) {
        glEnableVertexAttribArray((GLuint)i);
        glVertexAttribPointer((GLuint)i, vertexAttributes[i].ComponentCount, (GLenum)vertexAttributes[i].ComponentFormat, GL_FALSE, this->_VertexSize, (void*)offset);
        offset += vertexAttributes[i].ComponentCount * vertexAttributes[i].ComponentSize;
    }

    // Create the index buffer and upload the indices
    glGenBuffers(1, &this->_IndexBufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_IndexBufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_IndexSize * this->_IndexCount, indexData, GL_STATIC_DRAW);

    // Unbind the VAO so following code doesn't accidentally change it
    glBindVertexArray(0);

    if (indexSize == 2) {
        this->_IndexFormat = GL_UNSIGNED_SHORT;
    }
    else if (indexSize == 4) {
        this->_IndexFormat = GL_UNSIGNED_INT;
    }
}

GLMesh::~GLMesh() {
    glDeleteVertexArrays(1, &this->_VertexArrayHandle);
    this->_VertexArrayHandle = 0;
    glDeleteBuffers(1, &this->_VertexBufferHandle);
    this->_VertexBufferHandle = 0;
    glDeleteBuffers(1, &this->_IndexBufferHandle);
    this->_IndexBufferHandle = 0;
}