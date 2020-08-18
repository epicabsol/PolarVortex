#pragma once

#include "UIElement.h"

class Allocator;
class SpriteInstance;

class UIAnimation : public UIElement {
private:
    SpriteInstance* _Sprite;

protected:
    virtual Vector2 MeasureContent() const override;

public:
    inline UIAnimation(Allocator& allocator, SpriteInstance* sprite) : UIElement(allocator), _Sprite(sprite) { }
    ~UIAnimation();

    inline const SpriteInstance* GetSprite() const { return this->_Sprite; }
    inline SpriteInstance* GetSprite() { return this->_Sprite; }
    inline void SetSprite(SpriteInstance* sprite) { this->_Sprite = sprite; this->InvalidateContentSize(); this->InvalidateLayout(); }

    virtual void Render(const UIRectangle& absoluteParentBounds) override;
};