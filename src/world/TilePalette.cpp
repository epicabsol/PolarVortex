#include "world/TilePalette.h"

#include "assets/AssetManager.h"
#include "assets/sajson.h"
#include "memory/Allocator.h"

#define JSON_BUFFER_LENGTH (1024 * 4)

TilePalette::TilePalette(Allocator& allocator, const char* data, size_t dataLength) : _Allocator(allocator) {
    void* parseBuffer = allocator.Allocate(JSON_BUFFER_LENGTH);

    const sajson::document doc = sajson::parse<sajson::single_allocation, sajson::mutable_string_view>(sajson::single_allocation((size_t*)parseBuffer, JSON_BUFFER_LENGTH), sajson::mutable_string_view(dataLength, (char*)data));

    sajson::value rootObject = doc.get_root();
    sajson::value texturePath = rootObject.get_value_of_key(sajson::literal("texture"));
    Hash texturePathHash = HashData(texturePath.as_cstring(), texturePath.get_string_length());
    this->_Texture = Game->GetAssetManager().GetAsset(texturePathHash)->GetAsset<GLTexture>();

    float tileSize = rootObject.get_value_of_key(sajson::literal("tile-size")).get_number_value();
    this->_TileWidth = tileSize / this->_Texture->GetWidth();
    this->_TileHeight = tileSize / this->_Texture->GetHeight();

    allocator.Free(parseBuffer);
}