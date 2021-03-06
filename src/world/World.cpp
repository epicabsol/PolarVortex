#include "world/World.h"

#include <time.h>

#include "assets/Asset.h"
#include "assets/AssetManager.h"
#include "game/PolarVortexGame.h"
#include "render/GLTexture.h"
#include "world/Collision.h"
#include "world/Grid.h"
#include "world/GridBlueprint.h"
#include "world/Object.h"
#include "world/WorldBlueprint.h"

/**
 * @brief Reflects a 2D vector according to the given normal, with the given bias scaling each axis.
 *
 * The component of the result perpendicular to the given normal is scaled by the X component of the bias vector,
 * and the component of the result in the direction of the given normal is scaled by the Y component of the bias vector.
 *
 * This is especially helpful for faking friction and restitution.
 *
 * @param direction The input vector to reflect.
 * @param normal The direction around which the given direction should be reflected.
 * @param bias The scale of the result components in the perpendicular and parallel direction to the normal, respectively.
 * @return The reflected and biased vector.
 */
inline Vector2 BiasedReflect2D(const Vector2& direction, const Vector2& normal, const Vector2& bias) {
    return direction * bias.X - Math_Dot(direction, normal) * normal * (bias.X + bias.Y);
}

float World::StepDynamic(DynamicCollider& dynamic, float timestep) {
    Intersection closestIntersection;
    Collider* closestStatic = nullptr;
    World* closestWorld = nullptr;

    Vector2 goalPosition = dynamic._Bounds.Position + dynamic._Velocity * timestep;
    closestIntersection.Time = 1.0f;
    closestIntersection.EndPosition = goalPosition;
    closestIntersection.Overlap = Vector2();

    // Check for collision with each static collider
    for (Collider& collider : this->_ColliderPool) {
        Intersection intersection;
        if (SweptBoundingBoxIntersectsBoundingBox(dynamic._Bounds, goalPosition, collider.GetBounds(), intersection)) {
            if (intersection.Time < closestIntersection.Time) {
                closestStatic = &collider;
                closestWorld = nullptr;
                closestIntersection = intersection;
            }
        }
    }

    // Check for collision with tiles
    Intersection tileIntersection;
    if (SweptBoundingBoxIntersectsWorld(dynamic._Bounds, goalPosition, this, tileIntersection)) {
        if (tileIntersection.Time < closestIntersection.Time) {
            closestStatic = nullptr;
            closestWorld = this;
            closestIntersection = tileIntersection;
        }
    }

    dynamic._Bounds.Position = closestIntersection.EndPosition - closestIntersection.Overlap;
    dynamic._LastOnGround += closestIntersection.Time * timestep;
    if (closestStatic != nullptr) {
        Vector2 bias = Vector2(1.0f / (1.0f + dynamic.GetFriction() * closestStatic->GetFriction()), dynamic.GetRestitution() * closestStatic->GetRestitution());
        dynamic._Velocity = BiasedReflect2D(dynamic._Velocity, closestIntersection.Normal, bias);
        if (closestIntersection.Normal.X == 0.0f && closestIntersection.Normal.Y == 1.0f) {
            dynamic._LastOnGround = 0.0f;
        }
    }
    else if (closestWorld != nullptr) {
        float worldFriction = 0.5f;
        float worldRestitution = 0.1f;
        Vector2 bias = Vector2(1.0f / (1.0f + dynamic.GetFriction() * worldFriction), dynamic.GetRestitution() * worldRestitution);
        dynamic._Velocity = BiasedReflect2D(dynamic._Velocity, closestIntersection.Normal, bias);
        if (closestIntersection.Normal.X == 0.0f && closestIntersection.Normal.Y == 1.0f) {
            dynamic._LastOnGround = 0.0f;
        }
        dynamic._Bounds.Position += closestIntersection.Normal * HMM_EPSILON;
    }

    if (dynamic._Velocity.Y > 0.1f) {
        dynamic._LastOnGround = INFINITY;
    }

    return (1.0f - closestIntersection.Time) * timestep;
}

World::World(Allocator& allocator, const WorldBlueprint* blueprint) : _Allocator(allocator), _GridCount(blueprint->GetGridCount()), _Gravity(0.0f, -9.8f * 2.0f), _ColliderPool("World Collider Pool", this->_ColliderPoolBuffer, sizeof(Collider) * MAX_COLLIDERS), _DynamicColliderPool("World Dynamic Collider Pool", this->_DynamicColliderPoolBuffer, sizeof(DynamicCollider) * MAX_DYNAMIC_COLLIDERS) {
    this->_Grids = (Grid*)allocator.Allocate(sizeof(Grid) * blueprint->GetGridCount());
    for (size_t i = 0; i < blueprint->GetGridCount(); i++) {
        new (&this->_Grids[i]) Grid(allocator, &blueprint->GetGrids()[i]);
    }
}

World::~World() {
    for (size_t i = 0; i < this->_ObjectCount; i++) {
        this->_Objects[i]->Removed();
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

void World::AddObject(Object* object) {
    assert(this->_ObjectCount < MAX_OBJECTS);

    this->_Objects[this->_ObjectCount] = object;
    ++this->_ObjectCount;
    object->_World = this;
    object->Added();
}

void World::RemoveObject(Object* object, bool immediate) {
    object->Removed();
    object->_World = nullptr;
    if (immediate) {
        for (size_t i = 0; i < this->_ObjectCount; i++) {
            if (this->_Objects[i] == object) {
                this->_Objects[i] = this->_Objects[this->_ObjectCount - 1];
                --this->_ObjectCount;
                break;
            }
        }
    }
    else {
        object->_Remove = true;
    }
}

void World::Update(float timestep) {
    // Physics Step
    // 1. Update dynamic bodies
    for (DynamicCollider& dynamic : this->_DynamicColliderPool) {
        Vector2 acceleration = dynamic._StepForce / dynamic._Mass;
        dynamic._Velocity += acceleration * timestep;
    }
    // 2. Detect collisions & solve constraints
    for (DynamicCollider& dynamic : this->_DynamicColliderPool) {
        float time = timestep;
        int i = 0;
        while (time > HMM_EPSILON && i < 10) {
            time = this->StepDynamic(dynamic, time);
            i++;
        }
    }
    // 3. Reset forces
    for (DynamicCollider& dynamic : this->_DynamicColliderPool) {
        dynamic._StepForce = this->_Gravity * dynamic._Mass;
    }
    // 4. Update Objects
    for (size_t i = 0; i < this->_ObjectCount; i++) {
        this->_Objects[i]->Update(timestep);
    }
    // 5. Remove all Objects marked for removal
    for (size_t i = 0; i < this->_ObjectCount; i++) {
        if (this->_Objects[i]->_Remove) {
            this->_Objects[i] = this->_Objects[this->_ObjectCount - 1];
            --this->_ObjectCount;
            i--;
        }
    }
}

void World::Render(Camera* camera) {
    for (const Collider& collider : this->_ColliderPool) {
        Game->GetRenderer().DrawSprite(nullptr, collider.GetBounds().Position.X, collider.GetBounds().Position.Y, 0.0f, collider.GetBounds().HalfSize.X * 2.0f, collider.GetBounds().HalfSize.Y * 2.0f);
    }
    /*for (const DynamicCollider& collider : this->_DynamicColliderPool) {
        Game->GetRenderer().DrawSprite(this->_DirtTexture, collider.GetBounds().Position.X, collider.GetBounds().Position.Y, 0.0f, collider.GetBounds().HalfSize.X * 2.0f, collider.GetBounds().HalfSize.Y * 2.0f);
    }*/
    for (size_t i = 0; i < this->_ObjectCount; i++) {
        this->_Objects[i]->Render();
    }
}
