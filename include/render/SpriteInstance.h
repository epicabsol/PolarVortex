#pragma once

#include <stddef.h>

#include "SpriteAnimation.h"
#include "SpriteFrame.h"

class Allocator;

class SpriteInstance {
private:
    SpriteAnimation* _Animation;
    size_t _FrameIndex;
    float _FrameTime;

public:
    inline SpriteInstance(Allocator& allocator, SpriteAnimation* animation) : _Animation(animation), _FrameIndex(0), _FrameTime(0.0f) { }

    inline const GLTexture* GetTexture() const { return this->_Animation->GetTexture(); }
    inline const SpriteFrame& GetCurrentFrame() const { return this->_Animation->GetFrames()[this->_FrameIndex]; }

    void Advance(float timestep);
};