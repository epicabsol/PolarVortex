#pragma once

#define MAX_DYNAMIC_COLLIDERS (1 << 8)
#define MAX_COLLIDERS (1 << 10)
#define MAX_OBJECTS (1 << 8)

#include "Collider.h"
#include "DynamicCollider.h"
#include "../memory/IterablePoolAllocator.h"

class Camera;
class GLTexture;
class Object;
class TilePalette;

struct WorldTile {
    int16_t PaletteIndex;
    bool Collides;
};

class World {
private:
    Allocator& _Allocator;
    const TilePalette* _TilePalette;
    WorldTile* _Tiles;
    size_t _Width;
    size_t _Height;

    Vector2 _Gravity;
    IterablePoolAllocator<Collider> _ColliderPool;
    IterablePoolAllocator<DynamicCollider> _DynamicColliderPool;
    const GLTexture* _DirtTexture;

    char _ColliderPoolBuffer[sizeof(Collider) * MAX_COLLIDERS];
    char _DynamicColliderPoolBuffer[sizeof(DynamicCollider) * MAX_DYNAMIC_COLLIDERS];
    Object* _Objects[MAX_OBJECTS];
    size_t _ObjectCount;

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
    ~World();

    inline const TilePalette* GetTilePalette() const { return this->_TilePalette; }
    inline WorldTile& GetTile(size_t x, size_t y) { return this->_Tiles[y * this->_Width + x]; }
    inline const WorldTile& GetTile(size_t x, size_t y) const { return this->_Tiles[y * this->_Width + x]; }
    inline size_t GetWidth() const { return this->_Width; }
    inline size_t GetHeight() const { return this->_Height; }
    const WorldTile* GetTiles() const { return this->_Tiles; }

    Collider* AddCollider(Vector2 center, Vector2 halfSize);
    void RemoveCollider(Collider* collider);
    DynamicCollider* AddDynamicCollider(Vector2 center, Vector2 halfSize, float mass);
    void RemoveDynamicCollider(DynamicCollider* collider);
    void AddObject(Object* object);
    void RemoveObject(Object* object, bool immediate = false);

    void Update(float timestep);
    void Render(Camera* camera);
};