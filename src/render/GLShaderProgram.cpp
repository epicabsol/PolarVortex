#include "render/GLShaderProgram.h"

#include <stdio.h>

#include "assets/Asset.h"
#include "assets/AssetManager.h"
#include "render/glad.h"

void CheckShaderStatus(unsigned int shaderHandle) {
    int success;
    static char infoLog[512];
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderHandle, 512, nullptr, infoLog);
        printf("ERROR: Failed to compile shader. Log:\n%s\n", infoLog);
    }
}

void CheckProgramStatus(unsigned int programHandle) {
    int success;
    static char infoLog[512];
    glGetShaderiv(programHandle, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(programHandle, 512, nullptr, infoLog);
        printf("ERROR: Failed to link shader program. Log:\n%s\n", infoLog);
    }
}

void GLShaderProgram::LoadSource(const char* vertexShaderSource, int vertexShaderLength, const char* pixelShaderSource, int pixelShaderLength) {
    // Create and compile the vertex shader
    unsigned int vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderHandle, 1, &vertexShaderSource, &vertexShaderLength);
    glCompileShader(vertexShaderHandle);
    CheckShaderStatus(vertexShaderHandle);

    // Create and compile the pixel shader
    unsigned int pixelShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(pixelShaderHandle, 1, &pixelShaderSource, &pixelShaderLength);
    glCompileShader(pixelShaderHandle);
    CheckShaderStatus(pixelShaderHandle);

    // Create and link the program
    this->_ProgramHandle = glCreateProgram();
    glAttachShader(this->_ProgramHandle, vertexShaderHandle);
    glAttachShader(this->_ProgramHandle, pixelShaderHandle);
    glLinkProgram(this->_ProgramHandle);
    CheckProgramStatus(this->_ProgramHandle);

    // Delete shaders
    glDeleteShader(vertexShaderHandle);
    glDeleteShader(pixelShaderHandle);
}

GLShaderProgram::GLShaderProgram(const char* vertexShaderSource, int vertexShaderLength, const char* pixelShaderSource, int pixelShaderLength) {
    this->LoadSource(vertexShaderSource, vertexShaderLength, pixelShaderSource, pixelShaderLength);
}

GLShaderProgram::GLShaderProgram(Hash vertexShaderFilenameHash, Hash pixelShaderFilenameHash) {
    Asset* vertexShaderAsset = Assets->GetAsset(vertexShaderFilenameHash);
    Asset* pixelShaderAsset = Assets->GetAsset(pixelShaderFilenameHash);
    this->LoadSource((const char*)vertexShaderAsset->GetData(), (int)vertexShaderAsset->GetDataLength(), (const char*)pixelShaderAsset->GetData(), (int)pixelShaderAsset->GetDataLength());
}

GLShaderProgram::~GLShaderProgram() {
    glDeleteProgram(this->_ProgramHandle);
}