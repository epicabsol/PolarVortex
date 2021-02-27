#include "world/GridBlueprint.h"

#include "assets/AssetManager.h"
#include "assets/sajson.h"
#include "memory/Allocator.h"
#include "world/World.h"

GridBlueprint::GridBlueprint(Allocator& allocator, const Vector2& position, const TilePalette* palette, size_t width, size_t height, WorldTile* tiles) : _Allocator(allocator), _Position(position), _Palette(palette), _Width(width), _Height(height), _Tiles(tiles) {

}

GridBlueprint::~GridBlueprint() {
    this->_Allocator.Free(this->_Tiles);
}