#pragma once

#include "Collider.h"


class DynamicCollider : public Collider {
friend class World;

private:
    Vector2 _Velocity;
    Vector2 _StepForce; // Accumulates forces in the beginning of a physics step.
    float _Mass;

public:
    inline DynamicCollider(BoundingBox bounds, float mass) : Collider(bounds), _Mass(mass) { }
};