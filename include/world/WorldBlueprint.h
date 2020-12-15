#pragma once

class Allocator;
struct WorldTile;
class TilePalette;

class WorldBlueprint {
private:
    Allocator& _Allocator;
    const TilePalette* _Palette;
    size_t _Width;
    size_t _Height;
    WorldTile* _Tiles;

public:
    WorldBlueprint(Allocator& allocator, const char* data, size_t dataLength);
    ~WorldBlueprint();

    inline const TilePalette* GetTilePalette() const { return this->_Palette; }
    inline size_t GetWidth() const { return this->_Width; }
    inline size_t GetHeight() const { return this->_Height; }
    inline const WorldTile* GetTiles() const { return this->_Tiles; }
    inline WorldTile* GetTiles() { return this->_Tiles; }
};