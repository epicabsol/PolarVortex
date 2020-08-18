#pragma once

#include "UIContainer.h"

enum class DockSide {
    Left,
    Top,
    Right,
    Bottom
};

class UIDockContainer : public UIContainer<DockSide> {
private:

protected:
    virtual Vector2 MeasureContent() const override;

public:
    UIDockContainer(Allocator& allocator, size_t capacity) : UIContainer<DockSide>(allocator, capacity) { this->_HorizontalAlignment = UIAlignment::Stretch; this->_VerticalAlignment = UIAlignment::Stretch; }

    virtual void LayoutInto(const UIRectangle& destination) override;
};