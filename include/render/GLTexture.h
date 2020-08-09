#pragma once

#include "../Hash.h"

class GLTexture {
private:
    unsigned int _Handle;
    unsigned int _Width;
    unsigned int _Height;

    void LoadData(const void* data);

public:
    GLTexture(unsigned int width, unsigned int height, const void* data);
    GLTexture(Hash filenameHash);
    ~GLTexture();

    inline unsigned int GetHandle() const { return this->_Handle; }
    inline unsigned int GetWidth() const { return this->_Width; }
    inline unsigned int GetHeight() const { return this->_Height; }
};