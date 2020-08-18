#include "ui/UIAnimation.h"

#include "game/PolarVortexGame.h"
#include "memory/Allocator.h"
#include "render/SpriteInstance.h"
#include "render/GLRenderer.h"
#include "render/GLTexture.h"

Vector2 UIAnimation::MeasureContent() const {
    if (this->_Sprite == nullptr) {
        return Vector2();
    }
    else {
        return Vector2(this->_Sprite->GetCurrentFrame().Sprite.USize * this->_Sprite->GetTexture()->GetWidth(), this->_Sprite->GetCurrentFrame().Sprite.VSize * this->_Sprite->GetTexture()->GetHeight());
    }
}

UIAnimation::~UIAnimation() {
    if (this->_Sprite != nullptr) {
        this->_Allocator.Delete(this->_Sprite); // TODO: Delete from the correct allocator!
    }
}

void UIAnimation::Render(const UIRectangle& absoluteParentBounds) {
    if (this->_Sprite != nullptr) {
        Game->GetRenderer().DrawSprite(*this->_Sprite, absoluteParentBounds.Position.X + this->_Bounds.Position.X + this->_Bounds.Size.X * 0.5f, absoluteParentBounds.Position.Y + this->_Bounds.Position.Y + this->_Bounds.Size.Y * 0.5f, 0.0f, this->_Bounds.Size.X, this->_Bounds.Size.Y * -1.0f);
    }
}