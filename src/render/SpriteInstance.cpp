#include "render/SpriteInstance.h"

#include "render/SpriteFrame.h"

void SpriteInstance::Advance(float timestep) {
    this->_FrameTime += timestep * this->_TimeScale;
    while (this->_FrameTime > this->GetCurrentFrame().Duration) {
        this->_FrameTime -= this->GetCurrentFrame().Duration;
        this->_FrameIndex = (this->_FrameIndex + 1) % this->_Animation->GetFrameCount();
    }
}

void SpriteInstance::SetAnimation(const SpriteAnimation* animation, bool resetTime) {
    this->_Animation = animation;
    if (resetTime) {
        this->_FrameIndex = 0;
        this->_FrameTime = 0.0f;
    }
}
