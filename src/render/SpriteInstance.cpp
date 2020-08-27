#include "render/SpriteInstance.h"

#include "render/SpriteFrame.h"

void SpriteInstance::Advance(float timestep) {
    this->_FrameTime += timestep * this->_TimeScale;
    while (this->_FrameTime > this->GetCurrentFrame().Duration) {
        this->_FrameTime -= this->GetCurrentFrame().Duration;

        if (this->_FrameIndex == this->_Animation->GetFrameCount() - 1) {
            if (this->_Loop) {
                this->_FrameIndex = 0;
            }
            else {
                this->_FrameTime = 0.0f;
                break;
            }
            this->_FrameIndex = this->_FrameIndex % this->_Animation->GetFrameCount();
        }
        else {
            this->_FrameIndex = (this->_FrameIndex + 1);
        }
    }
}

void SpriteInstance::SetAnimation(const SpriteAnimation* animation, bool resetTime) {
    this->_Animation = animation;
    if (resetTime) {
        this->_FrameIndex = 0;
        this->_FrameTime = 0.0f;
    }
}
