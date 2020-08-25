#include "ui/UITextBlock.h"

#include <assert.h>

#include "game/PolarVortexGame.h"
#include "render/SpriteFont.h"

Vector2 UITextBlock::MeasureContent() const {
    if (this->_Font != nullptr) {
        return this->_Font->MeasureString(this->_Text, this->_Scale, 0.0f);
    }
    else {
        return Vector2();
    }
}

void UITextBlock::SetText(const char* text) {
    size_t length = strlen(text);
    assert(length < 255);
    strncpy_s(this->_Text, 255, text, 255);
    this->InvalidateContentSize();
}

void UITextBlock::Render(const UIRectangle& absoluteParentBounds) {
    //Game->GetRenderer().DrawSprite(nullptr, absoluteParentBounds.Position.X + this->_Bounds.Position.X + this->_Bounds.Size.X * 0.5f, absoluteParentBounds.Position.Y + this->_Bounds.Position.Y + this->_Bounds.Size.Y * 0.5f, 0.0f, this->_Bounds.Size.X, this->_Bounds.Size.Y);
    Game->GetRenderer().DrawString(this->_Font, this->_Text, absoluteParentBounds.Position.X + this->_Bounds.Position.X, absoluteParentBounds.Position.Y + this->_Bounds.Position.Y + this->_Bounds.Size.Y, 0.0f, this->_Scale, this->_Bounds.Size.X);
}