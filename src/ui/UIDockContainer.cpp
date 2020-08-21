#include "ui/UIDockContainer.h"

Vector2 UIDockContainer::MeasureContent() const {
    Vector2 result;
    UIMargins margins;

    for (const Child& child : this->_ChildAllocator) {
        Vector2 size = child.Element->Measure();
        switch (child.LayoutMeta) {
            case DockSide::Left: {
                margins.Left += size.X;
                result.Y = HMM_MAX(result.Y, margins.Top + margins.Bottom + size.Y);
                break;
            }
            case DockSide::Top: {
                margins.Bottom += size.Y;
                result.X = HMM_MAX(result.X, margins.Left + margins.Right + size.X);
                break;
            }
            case DockSide::Right: {
                margins.Right += size.X;
                result.Y = HMM_MAX(result.Y, margins.Top + margins.Bottom + size.Y);
                break;
            }
            case DockSide::Bottom: {
                margins.Top += size.Y;
                result.X = HMM_MAX(result.X, margins.Left + margins.Right + size.X);
                break;
            }
        }
    }

    result.X = HMM_MAX(result.X, margins.Left + margins.Right);
    result.Y = HMM_MAX(result.Y, margins.Top + margins.Bottom);

    return result;
}

void UIDockContainer::LayoutInto(const UIRectangle& destination) {
    UIContainer<DockSide>::LayoutInto(destination);

    // Layout children
    UIRectangle available = UIRectangle(Vector2(), this->_Bounds.Size);
    for (Child& child : this->_ChildAllocator) {
        switch (child.LayoutMeta) {
            case DockSide::Left: {
                float left = child.Element->Measure().X;

                child.Element->LayoutInto(UIRectangle(available.Position, Vector2(left, available.Size.Y)));

                available.Position.X += left;
                available.Size.X -= left;
                break;
            }
            case DockSide::Top: {
                float bottom = child.Element->Measure().Y;

                child.Element->LayoutInto(UIRectangle(Vector2(available.Position.X, available.Position.Y + available.Size.Y - bottom), Vector2(available.Size.X, bottom)));

                available.Size.Y -= bottom;
                break;
            }
            case DockSide::Right: {
                float right = child.Element->Measure().X;

                child.Element->LayoutInto(UIRectangle(Vector2(available.Position.X + available.Size.X - right, available.Position.Y), Vector2(right, available.Size.Y)));

                available.Size.X -= right;
                break;
            }
            case DockSide::Bottom: {
                float top = child.Element->Measure().Y;

                child.Element->LayoutInto(UIRectangle(available.Position, Vector2(available.Size.X, top)));

                available.Position.Y += top;
                available.Size.Y -= top;
                break;
            }
        }
    }
}