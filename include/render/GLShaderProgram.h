#pragma once

#include "../Hash.h"

class GLShaderProgram {
private:
    unsigned int _ProgramHandle;

    void LoadSource(const char* vertexShaderSource, int vertexShaderLength, const char* pixelShaderSource, int pixelShaderLength);

public:
    GLShaderProgram(const char* vertexShaderSource, int vertexShaderLength, const char* pixelShaderSource, int pixelShaderLength);
    GLShaderProgram(Hash vertexShaderFilenameHash, Hash pixelShaderFilenameHash);
    ~GLShaderProgram();

    inline unsigned int GetProgramHandle() const { return this->_ProgramHandle; }
};