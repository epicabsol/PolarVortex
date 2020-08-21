#include "ui/UIElement.h"

Vector2 UIElement::MeasureContent() const {
    return Vector2();
}

Vector2 UIElement::GetContentSize() {
    if (!this->_ContentSizeValid) {
        this->_ContentSize = this->MeasureContent();
        this->_ContentSizeValid = true;
    }
    return this->_ContentSize;
}

void UIElement::LayoutInto(const UIRectangle& destination) {
    Vector2 contentSize = this->GetContentSize();

    UIRectangle adjustedDestination = destination;
    adjustedDestination.Position.X += this->_Margins.Left;
    adjustedDestination.Position.Y += this->_Margins.Bottom;
    adjustedDestination.Size.X -= this->_Margins.Left + this->_Margins.Right;
    adjustedDestination.Size.Y -= this->_Margins.Top + this->_Margins.Bottom;

    // Align X
    switch (this->_HorizontalAlignment) {
    case UIAlignment::Near:
        this->_Bounds.Position.X = adjustedDestination.Position.X;
        this->_Bounds.Size.X = contentSize.X;
        break;
    case UIAlignment::Center:
        this->_Bounds.Position.X = adjustedDestination.Position.X + adjustedDestination.Size.X * 0.5f - contentSize.X * 0.5f;
        this->_Bounds.Size.X = contentSize.X;
        break;
    case UIAlignment::Far:
        this->_Bounds.Position.X = adjustedDestination.Position.X + adjustedDestination.Size.X - contentSize.X;
        this->_Bounds.Size.X = contentSize.X;
        break;
    case UIAlignment::Stretch:
        this->_Bounds.Position.X = adjustedDestination.Position.X;
        this->_Bounds.Size.X = adjustedDestination.Size.X;
        break;
    }

    // Align Y
    switch (this->_VerticalAlignment) {
    case UIAlignment::Near:
        this->_Bounds.Position.Y = adjustedDestination.Position.Y;
        this->_Bounds.Size.Y = contentSize.Y;
        break;
    case UIAlignment::Center:
        this->_Bounds.Position.Y = adjustedDestination.Position.Y + adjustedDestination.Size.Y * 0.5f - contentSize.Y * 0.5f;
        this->_Bounds.Size.Y = contentSize.Y;
        break;
    case UIAlignment::Far:
        this->_Bounds.Position.Y = adjustedDestination.Position.Y + adjustedDestination.Size.Y - contentSize.Y;
        this->_Bounds.Size.Y = contentSize.Y;
        break;
    case UIAlignment::Stretch:
        this->_Bounds.Position.Y = adjustedDestination.Position.Y;
        this->_Bounds.Size.Y = adjustedDestination.Size.Y;
        break;
    }
}

Vector2 UIElement::Measure() const {
    Vector2 contentSize = ((UIElement*)this)->GetContentSize();
    contentSize.X += this->_Margins.Left + this->_Margins.Right;
    contentSize.Y += this->_Margins.Top + this->_Margins.Bottom;
    return contentSize;
}

void UIElement::InvalidateLayout() {
    if (this->_Parent != nullptr) {
        this->_Parent->InvalidateLayout();
    }
    else {
        this->LayoutInto(this->_Bounds);
    }
}