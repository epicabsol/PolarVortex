#pragma once

#include "UIElement.h"
#include "../render/Sprite.h"

class Allocator;

class UISprite : public UIElement {
private:
    Sprite _Sprite;

protected:
    virtual Vector2 MeasureContent() const override;

public:
    inline UISprite(Allocator& allocator, const Sprite& sprite) : UIElement(allocator), _Sprite(sprite) { }
    virtual ~UISprite() { }

    virtual void Render(const UIRectangle& absoluteParentBounds) override;
};