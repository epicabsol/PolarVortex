#pragma once

#include <stddef.h>

#include "../Hash.h"

class Allocator;
class GLTexture;
struct SpriteFrame;

class SpriteAnimation {
private:
    Allocator& _Allocator;
    SpriteFrame* _Frames;
    size_t _FrameCount;

public:
    SpriteAnimation(Allocator& allocator, Hash layoutPathHash);
    ~SpriteAnimation();

    inline const SpriteFrame* GetFrames() const { return this->_Frames; }
    inline size_t GetFrameCount() const { return this->_FrameCount; }
};