#pragma once

#include "../memory/Memory.h"
#include "../render/GLTexture.h"

/**
 * @brief Abstract class that declares the functionality of all elements of an input device.
 *
 * Input Elements are parts of an input device that have a value from 0.0f to 1.0f.
 */
class InputElement {
private:
    GLTexture* _Sprite;

public:
    inline InputElement(Allocator& allocator, Hash spriteHash) : _Sprite(allocator.New<GLTexture>(spriteHash)) { }
    virtual ~InputElement() { }

    inline GLTexture* GetSprite() const { return this->_Sprite; }
    virtual const char* GetName() const = 0;
    virtual float GetValue() const = 0;
};