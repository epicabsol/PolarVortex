#include "world/WorldBlueprint.h"

#include "assets/AssetManager.h"
#include "assets/sajson.h"
#include "memory/Allocator.h"
#include "world/GridBlueprint.h"
#include "world/World.h"

// `sajson` guarantees that the parse buffer will worst-case be 1 word (4 bytes) per input character
#define JSON_BUFFER_LENGTH(inputLength) (inputLength * 4)

WorldBlueprint::WorldBlueprint(Allocator& allocator, const char* data, size_t dataLength) : _Allocator(allocator) {
    void* parseBuffer = allocator.Allocate(JSON_BUFFER_LENGTH(dataLength));

    const sajson::document doc = sajson::parse<sajson::single_allocation, sajson::mutable_string_view>(sajson::single_allocation((size_t*)parseBuffer, dataLength), sajson::mutable_string_view(dataLength, (char*)data));

    sajson::value rootObject = doc.get_root();
    sajson::value gridArray = rootObject.get_value_of_key(sajson::literal("grids"));
    this->_GridCount = gridArray.get_length();
    this->_Grids = (GridBlueprint*)allocator.Allocate(sizeof(GridBlueprint) * this->_GridCount);

    for (size_t j = 0; j < this->_GridCount; j++) {
        sajson::value gridObject = gridArray.get_array_element(j);

        float x = gridObject.get_value_of_key(sajson::literal("x")).get_number_value();
        float y = gridObject.get_value_of_key(sajson::literal("y")).get_number_value();

        sajson::value palettePath = gridObject.get_value_of_key(sajson::literal("palette"));
        Hash palettePathHash = HashData(palettePath.as_cstring(), (uint32_t)palettePath.get_string_length());
        const TilePalette* palette = Game->GetAssetManager().GetAsset(palettePathHash)->GetAsset<TilePalette>();

        size_t width = (size_t)gridObject.get_value_of_key(sajson::literal("width")).get_integer_value();
        size_t height = (size_t)gridObject.get_value_of_key(sajson::literal("height")).get_integer_value();

        sajson::value tileArray = gridObject.get_value_of_key(sajson::literal("tiles"));
        WorldTile* tiles = (WorldTile*)allocator.Allocate(sizeof(WorldTile) * width * height);
        for (size_t i = 0; i < tileArray.get_length(); i++) {
            sajson::value tileObject = tileArray.get_array_element(i);

            tiles[i].PaletteIndex = tileObject.get_value_of_key(sajson::literal("index")).get_integer_value();
            tiles[i].Collides = tileObject.get_value_of_key(sajson::literal("collides")).get_boolean_value();
        }

        new (&this->_Grids[j]) GridBlueprint(allocator, Vector2(x, y), palette, width, height, tiles);
    }

    allocator.Free(parseBuffer);
}

WorldBlueprint::~WorldBlueprint() {
    this->_Allocator.Free(this->_Grids);
}