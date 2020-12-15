#include "world/WorldBlueprint.h"

#include "assets/AssetManager.h"
#include "assets/sajson.h"
#include "memory/Allocator.h"
#include "world/World.h"

// `sajson` guarantees that the parse buffer will worst-case be 1 word (4 bytes) per input character
#define JSON_BUFFER_LENGTH(inputLength) (inputLength * 4)

WorldBlueprint::WorldBlueprint(Allocator& allocator, const char* data, size_t dataLength) : _Allocator(allocator) {
    void* parseBuffer = allocator.Allocate(JSON_BUFFER_LENGTH(dataLength));

    const sajson::document doc = sajson::parse<sajson::single_allocation, sajson::mutable_string_view>(sajson::single_allocation((size_t*)parseBuffer, dataLength), sajson::mutable_string_view(dataLength, (char*)data));

    sajson::value rootObject = doc.get_root();
    sajson::value palettePath = rootObject.get_value_of_key(sajson::literal("palette"));
    Hash palettePathHash = HashData(palettePath.as_cstring(), (uint32_t)palettePath.get_string_length());
    this->_Palette = Game->GetAssetManager().GetAsset(palettePathHash)->GetAsset<TilePalette>();

    this->_Width = (size_t)rootObject.get_value_of_key(sajson::literal("width")).get_integer_value();
    this->_Height = (size_t)rootObject.get_value_of_key(sajson::literal("height")).get_integer_value();

    sajson::value tileArray = rootObject.get_value_of_key(sajson::literal("tiles"));
    this->_Tiles = (WorldTile*)allocator.Allocate(sizeof(WorldTile) * this->_Width * this->_Height);
    for (size_t i = 0; i < tileArray.get_length(); i++) {
        sajson::value tileObject = tileArray.get_array_element(i);

        this->_Tiles[i].PaletteIndex = tileObject.get_value_of_key(sajson::literal("index")).get_integer_value();
        this->_Tiles[i].Collides = tileObject.get_value_of_key(sajson::literal("collides")).get_boolean_value();
    }

    allocator.Free(parseBuffer);
}

WorldBlueprint::~WorldBlueprint() {
    this->_Allocator.Free(this->_Tiles);
}