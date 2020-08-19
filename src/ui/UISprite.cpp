#include "ui/UISprite.h"

#include "game/PolarVortexGame.h"
#include "render/GLTexture.h"

Vector2 UISprite::MeasureContent() const {
    if (this->_Sprite.Texture != nullptr) {
        return Vector2(this->_Sprite.Texture->GetWidth(), this->_Sprite.Texture->GetHeight());
    }
    else {
        return Vector2();
    }
}

void UISprite::Render(const UIRectangle& absoluteParentBounds) {
    if (this->_Sprite.Texture != nullptr) {
        Game->GetRenderer().DrawSprite(this->_Sprite, absoluteParentBounds.Position.X + this->_Bounds.Position.X + this->_Bounds.Size.X * 0.5f, absoluteParentBounds.Position.Y + this->_Bounds.Position.Y + this->_Bounds.Size.Y * 0.5f, 0.0f, this->_Bounds.Size.X, this->_Bounds.Size.Y * -1.0f);
    }
}