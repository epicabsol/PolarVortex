#pragma once

#include "UIElement.h"

#include "../memory/IterablePoolAllocator.h"

/**
 * @brief The base class of UI elements which layout child elements.
 * 
 * @tparam TLayoutMeta The type of layout metadata that this container uses with its child elements.
 */
template <typename TLayoutMeta>
class UIContainer : public UIElement {
protected:
    struct Child {
        Child* PreviousChild;
        Child* NextChild;
        TLayoutMeta LayoutMeta;
        UIElement* Element;
    };

    void* _ChildAllocatorBuffer;
    IterablePoolAllocator<Child> _ChildAllocator;
    Child* _FirstChild;

public:
    UIContainer(Allocator& allocator, size_t capacity) : UIElement(allocator), _ChildAllocatorBuffer(allocator.Allocate(capacity * sizeof(Child))), _ChildAllocator("UIContainer Child Pool", this->_ChildAllocatorBuffer, capacity * sizeof(Child)), _FirstChild(nullptr) { }
    ~UIContainer() { this->_Allocator.Free(this->_ChildAllocatorBuffer); }

    void AddElement(UIElement* element, const TLayoutMeta& layoutMeta) {
        Child* child = (Child*)this->_ChildAllocator.Allocate(sizeof(Child));
        child->Element = element;
        child->LayoutMeta = layoutMeta;
        element->_ChildMeta = child;
        element->_Parent = this;

        child->NextChild = this->_FirstChild;
        if (this->_FirstChild != nullptr) {
            this->_FirstChild->PreviousChild = child;
        }
        this->_FirstChild = child;

        this->InvalidateContentSize();
        this->InvalidateLayout();
    }

    void RemoveElement(UIElement* element) {
        Child* child = (Child*)element->_ChildMeta;

        if (child->NextChild != nullptr) {
            child->NextChild->PreviousChild = child->PreviousChild;
        }
        if (child->PreviousChild != nullptr) {
            child->PreviousChild->NextChild = child->NextChild;
        }
        if (this->_FirstChild == child) {
            this->_FirstChild = child->NextChild;
        }

        element->_Parent = nullptr;

        this->InvalidateContentSize();
        this->InvalidateLayout();
    }

    virtual void Render(const UIRectangle& absoluteParentBounds) override {
        UIRectangle absoluteBounds = absoluteParentBounds;
        absoluteBounds.Position += this->_Bounds.Position;
        absoluteBounds.Size = this->_Bounds.Size;
        for (Child& child : this->_ChildAllocator) {
            child.Element->Render(absoluteBounds);
        }
    }
};