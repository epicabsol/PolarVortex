#pragma once

#include <stdint.h>
#include "render/Math.h"

class Allocator;
class GridBlueprint;
class TilePalette;

struct WorldTile {
    int16_t PaletteIndex;
    bool Collides;
};

class Grid {
private:
    Allocator& _Allocator;
    Vector2 _Position;
    const TilePalette* _TilePalette;
    WorldTile* _Tiles;
    size_t _Width;
    size_t _Height;

public:
    Grid(Allocator& allocator, const GridBlueprint* blueprint);
    ~Grid();

    inline const Vector2& GetPosition() const { return this->_Position; }
    inline const TilePalette* GetTilePalette() const { return this->_TilePalette; }
    inline WorldTile& GetTile(size_t x, size_t y) { return this->_Tiles[y * this->_Width + x]; }
    inline const WorldTile& GetTile(size_t x, size_t y) const { return this->_Tiles[y * this->_Width + x]; }
    inline size_t GetWidth() const { return this->_Width; }
    inline size_t GetHeight() const { return this->_Height; }
    const WorldTile* GetTiles() const { return this->_Tiles; }
};