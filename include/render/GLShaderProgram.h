#pragma once

#include "../Hash.h"

class Allocator;

class GLShaderProgram {
private:
    unsigned int _ProgramHandle;

    void LoadSource(const char* vertexShaderSource, int vertexShaderLength, const char* pixelShaderSource, int pixelShaderLength);

public:
    GLShaderProgram(Allocator& allocator, const char* vertexShaderSource, int vertexShaderLength, const char* pixelShaderSource, int pixelShaderLength);
    GLShaderProgram(Allocator& allocator, Hash vertexShaderFilenameHash, Hash pixelShaderFilenameHash);
    ~GLShaderProgram();

    inline unsigned int GetProgramHandle() const { return this->_ProgramHandle; }
    void SetTextureIndex(const char* samplerName, unsigned int textureIndex);
};