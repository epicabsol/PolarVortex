#pragma once

#include "UIRectangle.h"
#include "UIAlignment.h"
#include "UIMargins.h"

class Allocator;

class UIElement {
private:
    Vector2 _ContentSize;
    bool _ContentSizeValid;

protected:
    Allocator& _Allocator;
    UIRectangle _Bounds;
    UIMargins _Margins;
    UIElement* _Parent;
    UIAlignment _HorizontalAlignment;
    UIAlignment _VerticalAlignment;

    /**
     * @brief Called when the element's cached content size is invalidated, and so the content must be measured.
     * 
     * @return The size of this element's content.
     */
    virtual Vector2 MeasureContent() const;

    Vector2 GetContentSize();

    inline void InvalidateContentSize() { this->_ContentSizeValid = false; }

public:
    inline UIElement(Allocator& allocator, UIElement* parent) : _Allocator(allocator), _ContentSize(), _ContentSizeValid(false), _Bounds(), _Margins(), _Parent(parent), _HorizontalAlignment(UIAlignment::Near), _VerticalAlignment(UIAlignment::Near) { }
    virtual ~UIElement() { }

    inline Allocator& GetAllocator() const { return this->_Allocator; }
    inline const UIRectangle& GetBounds() const { return this->_Bounds; }
    inline const UIMargins& GetMargins() const { return this->_Margins; }
    inline void SetMargins(const UIMargins& margins) { this->_Margins = margins; this->InvalidateLayout(); }
    inline UIElement* GetParent() { return this->_Parent; }
    inline UIAlignment GetHorizontalAlignment() const { return this->_HorizontalAlignment; }
    inline void SetHorizontalAlignment(UIAlignment horizontalAlignment) { this->_HorizontalAlignment = horizontalAlignment; this->InvalidateLayout(); }
    inline UIAlignment GetVerticalAlignment() const { return this->_VerticalAlignment; }
    inline void SetVerticalAlignment(UIAlignment verticalAlignment) { this->_VerticalAlignment = verticalAlignment; this->InvalidateLayout(); }

    /**
     * @brief Positions this element according to its ideal size, margins, and alignment, inside the given destination rectangle.
     * 
     * @param destination The rectangle to position this element within.
     */
    void LayoutInto(const UIRectangle& destination);

    /**
     * @brief Determines the ideal size of this element, including both content and margins.
     * 
     * @return The ideal size of this element.
     */
    Vector2 Measure() const;

    void InvalidateLayout();

    virtual void Render(const UIRectangle& absoluteParentBounds) { }
};