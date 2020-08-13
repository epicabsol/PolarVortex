#pragma once

#include "BoundingBox.h"

/**
 * @brief An axis-aligned bounding box that can be collided with.
 */
class Collider {
protected:
    BoundingBox _Bounds;
    // TODO: Physics Material, event listener

public:
    inline Collider(const BoundingBox& bounds) : _Bounds(bounds) { }

    const BoundingBox& GetBounds() const { return this->_Bounds; }
    BoundingBox& GetBounds() { return this->_Bounds; }
};