#pragma once

#include "../assets/Asset.h"
#include "../assets/AssetManager.h"
#include "../game/PolarVortexGame.h"
#include "../memory/Memory.h"

class GLTexture;

/**
 * @brief Abstract class that declares the functionality of all elements of an input device.
 *
 * Input Elements are parts of an input device that have a value from 0.0f to 1.0f.
 */
class InputElement {
private:
    const GLTexture* _Sprite;

public:
    inline InputElement(Allocator& allocator, Hash spriteHash) : _Sprite(Game->GetAssetManager().GetAsset(spriteHash)->GetAsset<GLTexture>()) { }
    virtual ~InputElement() { }

    inline const GLTexture* GetSprite() const { return this->_Sprite; }
    virtual const char* GetName() const = 0;
    virtual float GetValue() const = 0;
};