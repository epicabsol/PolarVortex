#pragma once

#include "render/Math.h"
#include "world/Grid.h"

class Allocator;
struct WorldTile;
class TilePalette;

class GridBlueprint {
private:
    Allocator& _Allocator;
    Vector2 _Position;
    const TilePalette* _Palette;
    size_t _Width;
    size_t _Height;
    WorldTile* _Tiles;

public:
    GridBlueprint(Allocator& allocator, const Vector2& position, const TilePalette* palette, size_t width, size_t height, WorldTile* tiles);
    ~GridBlueprint();

    inline const Vector2& GetPosition() const { return this->_Position; }
    inline const TilePalette* GetTilePalette() const { return this->_Palette; }
    inline size_t GetWidth() const { return this->_Width; }
    inline size_t GetHeight() const { return this->_Height; }
    inline const WorldTile* GetTiles() const { return this->_Tiles; }
    inline WorldTile* GetTiles() { return this->_Tiles; }
};