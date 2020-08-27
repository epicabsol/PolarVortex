#pragma once

#include <stddef.h>

#include "SpriteAnimation.h"
#include "SpriteFrame.h"

class Allocator;

class SpriteInstance {
private:
    const SpriteAnimation* _Animation;
    size_t _FrameIndex;
    float _FrameTime;
    float _TimeScale;
    bool _Loop;

public:
    inline SpriteInstance(Allocator& allocator, const SpriteAnimation* animation) : _Animation(animation), _FrameIndex(0), _FrameTime(0.0f), _TimeScale(1.0f), _Loop(true) { }

    inline const SpriteAnimation* GetAnimation() const { return this->_Animation; }
    inline const GLTexture* GetTexture() const { return this->GetCurrentFrame().Sprite.Texture; }
    inline const SpriteFrame& GetCurrentFrame() const { return this->_Animation->GetFrames()[this->_FrameIndex]; }
    inline void SetFrameIndex(size_t frameIndex) { this->_FrameIndex = frameIndex; }
    inline float GetTimeScale() const { return this->_TimeScale; }
    inline void SetTimeScale(float timeScale) { this->_TimeScale = timeScale; }
    inline bool GetLoop() const { return this->_Loop; }
    inline void SetLoop(bool loop) { this->_Loop = loop; }

    void Advance(float timestep);
    void SetAnimation(const SpriteAnimation* animation, bool resetTime = true);
};