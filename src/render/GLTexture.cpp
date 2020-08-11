#include "render/GLTexture.h"

#include "assets/Asset.h"
#include "render/glad.h"
#include "render/stb_image.h"
#include "game/PolarVortexGame.h"

void GLTexture::LoadData(const void* data) {
    glGenTextures(1, &this->_Handle);
    glBindTexture(GL_TEXTURE_2D, this->_Handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->_Width, this->_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
}

GLTexture::GLTexture(unsigned int width, unsigned int height, const void* data) : _Handle(0), _Width(width), _Height(height) {
    this->LoadData(data);
}

GLTexture::GLTexture(Hash filenameHash) : _Handle(0), _Width(0), _Height(0) {
    Asset* textureAsset = Game->GetAssetManager().GetAsset(filenameHash);
    int width;
    int height;
    int channelsInFile;
    unsigned char* data = stbi_load_from_memory((unsigned char*)textureAsset->GetData(), (int)textureAsset->GetDataLength(), &width, &height, &channelsInFile, 4);
    this->_Width = width;
    this->_Height = height;
    this->LoadData(data);
    stbi_image_free(data);
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &this->_Handle);
    this->_Handle = 0;
}