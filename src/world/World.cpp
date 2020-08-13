#include "world/World.h"

#include "game/PolarVortexGame.h"
#include "world/Collision.h"

inline Vector2 Reflect2D(Vector2 direction, Vector2 normal) {
    return direction - 2.0f * Math_Dot(direction, normal) * normal;
}

World::World() : _Gravity(0.0f, -0.98f), _ColliderPool("World Collider Pool", this->_ColliderPoolBuffer, sizeof(Collider) * MAX_COLLIDERS), _DynamicColliderPool("World Dynamic Collider Pool", this->_DynamicColliderPoolBuffer, sizeof(DynamicCollider) * MAX_DYNAMIC_COLLIDERS) {

    // TEMP: Add some test colliders.
    this->AddCollider(Vector2(0.0f, 0.0f), Vector2(5.0f, 0.5f));
    this->AddCollider(Vector2(-5.0f, 1.0f), Vector2(0.25f, 0.25f));

    for (float f = -4.0f; f <= 5.5f; f += 1.0f) {
        this->AddDynamicCollider(Vector2(f, 4.0f), Vector2(0.25f, 0.25f), 0.125f)->_Velocity = Vector2(0.5f, 0.0f);
    }
}

Collider* World::AddCollider(Vector2 center, Vector2 halfSize) {
    return this->_ColliderPool.New<Collider>(BoundingBox(center, halfSize));
}

void World::RemoveCollider(Collider* collider) {
    this->_ColliderPool.Delete(collider);
}

DynamicCollider* World::AddDynamicCollider(Vector2 center, Vector2 halfSize, float mass) {
    return this->_DynamicColliderPool.New<DynamicCollider>(BoundingBox(center, halfSize), mass);
}

void World::RemoveDynamicCollider(DynamicCollider* collider) {
    this->_DynamicColliderPool.Delete(collider);
}

void World::Update(float timestep) {
    // Physics Step
    // 1. Apply forces
    for (DynamicCollider& dynamic : this->_DynamicColliderPool) {
        dynamic._StepForce = this->_Gravity;
    }
    // 2. Update dynamic bodies
    for (DynamicCollider& dynamic : this->_DynamicColliderPool) {
        Vector2 acceleration = dynamic._StepForce / dynamic._Mass;
        dynamic._Velocity += acceleration * timestep;
    }
    // 3. Detect collisions & solve constraints
    for (DynamicCollider& dynamic : this->_DynamicColliderPool) {
        Intersection closestIntersection;
        Vector2 goalPosition = dynamic._Bounds.Position + dynamic._Velocity * timestep;
        closestIntersection.Time = 1.0f;
        closestIntersection.EndPosition = goalPosition;
        closestIntersection.Overlap = Vector2();

        for (Collider& collider : this->_ColliderPool) {
            Intersection intersection;
            if (SweptBoundingBoxIntersectsBoundingBox(dynamic._Bounds, dynamic._Bounds.Position, goalPosition, collider.GetBounds(), intersection)) {
                if (intersection.Time < closestIntersection.Time) {
                    closestIntersection = intersection;
                    dynamic._Velocity = Reflect2D(dynamic._Velocity, intersection.Normal);
                }
            }
        }

        dynamic._Bounds.Position = closestIntersection.EndPosition - closestIntersection.Overlap;
    }
}

void World::Render(Camera* camera) {
    for (const Collider& collider : this->_ColliderPool) {
        Game->GetRenderer().DrawSprite(nullptr, collider.GetBounds().Position.X, collider.GetBounds().Position.Y, 0.0f, collider.GetBounds().HalfSize.X * 2.0f, collider.GetBounds().HalfSize.Y * 2.0f);
    }
    for (const DynamicCollider& collider : this->_DynamicColliderPool) {
        Game->GetRenderer().DrawSprite(nullptr, collider.GetBounds().Position.X, collider.GetBounds().Position.Y, 0.0f, collider.GetBounds().HalfSize.X * 2.0f, collider.GetBounds().HalfSize.Y * 2.0f);
    }
}