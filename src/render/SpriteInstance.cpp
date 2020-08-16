#include "render/SpriteInstance.h"

#include "render/SpriteFrame.h"

void SpriteInstance::Advance(float timestep) {
    this->_FrameTime += timestep;
    while (this->_FrameTime > this->GetCurrentFrame().Duration) {
        this->_FrameTime -= this->GetCurrentFrame().Duration;
        this->_FrameIndex = (this->_FrameIndex + 1) % this->_Animation->GetFrameCount();
    }
}