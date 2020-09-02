#pragma once

#include "UIMargins.h"
#include "UITextBlock.h"
#include "render/Sprite.h"

class UIButton : public UITextBlock {
private:
    UIMargins _Padding;
    Sprite _NormalSprite;

protected:
    virtual Vector2 MeasureContent() const override;

public:
    UIButton(Allocator& allocator, const SpriteFont* font);
    ~UIButton() { }

    inline const UIMargins& GetPadding() const { return this->_Padding; }
    inline void SetPadding(const UIMargins& padding) { this->_Padding = padding; }

    virtual void Render(const UIRectangle& absoluteParentBounds) override;
};