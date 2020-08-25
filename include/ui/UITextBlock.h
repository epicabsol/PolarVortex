#pragma once

#include "UIElement.h"

class SpriteFont;

class UITextBlock : public UIElement {
private:
    const SpriteFont* _Font;
    char _Text[255];
    float _Scale;

protected:
    virtual Vector2 MeasureContent() const override;

public:
    inline UITextBlock(Allocator& allocator, const SpriteFont* font) : UIElement(allocator), _Font(font), _Scale(1.0f) { }
    ~UITextBlock() { }

    inline const char* GetText() const { return this->_Text; }
    void SetText(const char* text);
    inline const SpriteFont* GetFont() const { return this->_Font; }
    inline void SetFont(const SpriteFont* font) { this->_Font = font; this->InvalidateContentSize(); }
    inline float GetScale() const { return this->_Scale; }
    inline void SetScale(float scale) { this->_Scale = scale; this->InvalidateContentSize(); }

    virtual void Render(const UIRectangle& absoluteParentBounds) override;
};