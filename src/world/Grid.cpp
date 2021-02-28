#include "world/Grid.h"

#include "memory/Allocator.h"
#include "world/GridBlueprint.h"

Grid::Grid(Allocator& allocator, const GridBlueprint* blueprint) : _Allocator(allocator), _Position(blueprint->GetPosition()), _TilePalette(blueprint->GetTilePalette()), _Tiles(nullptr), _Width(blueprint->GetWidth()), _Height(blueprint->GetHeight()) {
    this->_Tiles = (WorldTile*)allocator.Allocate(sizeof(WorldTile) * this->_Width * this->_Height);
    for (size_t y = 0; y < this->_Height; y++) {
        for (size_t x = 0; x < this->_Width; x++) {
            /*bool r =  false; //rand() % (2 + y) == 0;
            this->GetTile(x, y).Collides = (y == 0) || r;
            this->GetTile(x, y).PaletteIndex = (y == 0) ? 5 : (r ? 5 : -1);*/
            this->GetTile(x, y) = blueprint->GetTiles()[y * this->_Width + x];
        }
    }
}

Grid::~Grid() {
    this->_Allocator.Free(this->_Tiles);
    this->_Tiles = nullptr;
}