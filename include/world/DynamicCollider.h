#pragma once

#include "Collider.h"


class DynamicCollider : public Collider {
friend class World;

private:
    Vector2 _Velocity;
    Vector2 _StepForce; // Accumulates forces in the beginning of a physics step.
    float _Mass;

public:
    inline DynamicCollider(BoundingBox bounds, float mass) : Collider(bounds), _Velocity(0.0f, 0.0f), _StepForce(0.0f, 0.0f), _Mass(mass) { }

    inline Vector2 GetVelocity() const { return this->_Velocity; }
    inline Vector2& GetVelocity() { return this->_Velocity; }
    inline float GetMass() const { return this->_Mass; }
    inline void SetMass(float mass) { this->_Mass = mass; }
    inline void AddForce(Vector2 force) { this->_StepForce += force; }
};