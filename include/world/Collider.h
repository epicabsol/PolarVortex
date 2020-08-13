#pragma once

#include "BoundingBox.h"

/**
 * @brief An axis-aligned bounding box that can be collided with.
 */
class Collider {
protected:
    BoundingBox _Bounds;
    float _Restitution;
    float _Friction;

public:
    inline Collider(const BoundingBox& bounds) : _Bounds(bounds), _Restitution(1.0f), _Friction(1.0f) { }

    inline const BoundingBox& GetBounds() const { return this->_Bounds; }
    inline BoundingBox& GetBounds() { return this->_Bounds; }
    inline float GetRestitution() const { return this->_Restitution; }
    inline void SetRestitution(float restitution) { this->_Restitution = restitution; }
    inline float GetFriction() const { return this->_Friction; }
    inline void SetFriction(float friction) { this->_Friction = friction; }
};