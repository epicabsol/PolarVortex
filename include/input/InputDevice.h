#pragma once

#include <stddef.h>

class InputElement;
class GLTexture;

class InputDevice {
private:

public:
    virtual ~InputDevice() { }

    virtual const GLTexture* GetSprite() const = 0;
    virtual const char* GetName() const = 0;
    virtual bool IsConnected() const = 0;
    virtual const InputElement* const* GetInputElements() const = 0;
    inline const InputElement* GetInputElement(size_t index) const { return this->GetInputElements()[index]; }
    virtual size_t GetInputElementCount() const = 0;
};