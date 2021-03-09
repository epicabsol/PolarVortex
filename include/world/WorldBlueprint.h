#pragma once

#include <stddef.h>

class Allocator;
class GridBlueprint;

class WorldBlueprint {
private:
    Allocator& _Allocator;
    size_t _GridCount;
    GridBlueprint* _Grids;

public:
    WorldBlueprint(Allocator& allocator, const char* data, size_t dataLength);
    ~WorldBlueprint();

    inline size_t GetGridCount() const { return this->_GridCount; }
    inline const GridBlueprint* GetGrids() const { return this->_Grids; }
    inline GridBlueprint* GetGrids() { return this->_Grids; }
};