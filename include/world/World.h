#pragma once

#define MAX_DYNAMIC_COLLIDERS (1 << 8)
#define MAX_COLLIDERS (1 << 10)

#include "Collider.h"
#include "DynamicCollider.h"
#include "../memory/IterablePoolAllocator.h"

class Camera;
class GLTexture;

class World {
private:
    Vector2 _Gravity;
    IterablePoolAllocator<Collider> _ColliderPool;
    IterablePoolAllocator<DynamicCollider> _DynamicColliderPool;
    const GLTexture* _DirtTexture;

    char _ColliderPoolBuffer[sizeof(Collider) * MAX_COLLIDERS];
    char _DynamicColliderPoolBuffer[sizeof(DynamicCollider) * MAX_DYNAMIC_COLLIDERS];

    /**
     * @brief Moves the given dynamic collider according to its velocity for up to the given amount of time, or until it collides.
     *
     * @param dynamic The dynamic collider to simulate.
     * @param timestep The maximum amount of time to simulate.
     * @return float The amount of time remaining after a collision was found, if any.
     */
    float StepDynamic(DynamicCollider& dynamic, float timestep);

public:
    World(Allocator& allocator);

    Collider* AddCollider(Vector2 center, Vector2 halfSize);
    void RemoveCollider(Collider* collider);
    DynamicCollider* AddDynamicCollider(Vector2 center, Vector2 halfSize, float mass);
    void RemoveDynamicCollider(DynamicCollider* collider);

    void Update(float timestep);
    void Render(Camera* camera);
};