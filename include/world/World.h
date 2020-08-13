#pragma once

#define MAX_DYNAMIC_COLLIDERS (1 << 8)
#define MAX_COLLIDERS (1 << 10)

#include "Collider.h"
#include "DynamicCollider.h"
#include "../memory/IterablePoolAllocator.h"

class Camera;

class World {
private:
    Vector2 _Gravity;
    IterablePoolAllocator<Collider> _ColliderPool;
    IterablePoolAllocator<DynamicCollider> _DynamicColliderPool;

    char _ColliderPoolBuffer[sizeof(Collider) * MAX_COLLIDERS];
    char _DynamicColliderPoolBuffer[sizeof(DynamicCollider) * MAX_DYNAMIC_COLLIDERS];

public:
    World();

    Collider* AddCollider(Vector2 center, Vector2 halfSize);
    void RemoveCollider(Collider* collider);
    DynamicCollider* AddDynamicCollider(Vector2 center, Vector2 halfSize, float mass);
    void RemoveDynamicCollider(DynamicCollider* collider);

    void Update(float timestep);
    void Render(Camera* camera);
};