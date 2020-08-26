#pragma once

class Allocator;

/**
 * @brief Object is the base class for misc. things that are part of a world, and can update and render.
 */
class Object {
friend class World;
protected:
    Allocator& _Allocator;
    World* _World;
    bool _Remove;

    virtual void Added() { }
    virtual void Update(float timestep) { }
    virtual void Render() { }
    virtual void Removed() { }

public:
    Object(Allocator& allocator) : _Allocator(allocator), _World(nullptr), _Remove(false) { }
    virtual ~Object() { }

    inline World* GetWorld() const { return this->_World; }
};