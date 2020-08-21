#include "assets/AssetManager.h"

#include <filesystem>
#include <fstream>

#include "assets/Asset.h"
#include "memory/Memory.h"
#include "render/GLTexture.h"
#include "render/SpriteAnimation.h"
#include "render/SpriteFont.h"

#define ASSET_MAX_COUNT 64
#define DATA_ALLOCATOR_SIZE (1024 * 1024 * 32)
#define ASSET_DIRECTORY "assets"

AssetManager::AssetManager(Allocator& allocator) : _Allocator(allocator), _Assets((Asset*)allocator.Allocate(sizeof(Asset) * ASSET_MAX_COUNT)), _AssetCount(0) {

    using namespace std::filesystem;
    path assetDirectoryPath = path(ASSET_DIRECTORY);
    printf("Loading assets...\n");
    for (const directory_entry& entry : recursive_directory_iterator(assetDirectoryPath)) {

        if (entry.status().type() != file_type::regular) {
            continue;
        }

        if (this->_AssetCount >= ASSET_MAX_COUNT) {
            printf("ERROR: Too many assets. Increase ASSET_MAX_COUNT.\n");
            continue;
        }

        std::string pathString = entry.path().string();
        std::string extension = entry.path().extension().string();
        for (size_t i = 0; i < pathString.size(); i++) {
            if (pathString[i] == '\\') {
                pathString[i] = '/';
            }
        }
        printf("  %s\n", pathString.c_str());
        size_t length = file_size(entry.path());
        char* data = (char*)AssetsAllocator.Allocate(length);

        std::ifstream stream;
        stream.open(entry.path(), std::ios::binary);
        stream.read(data, length);
        stream.close();

        Hash pathHash = HashString(pathString);
        Hash typeHash = HashString(extension);

        new (&this->_Assets[this->_AssetCount]) Asset(allocator, pathHash, typeHash, data, length, nullptr);
        this->_AssetCount += 1;
    }
}

Asset* AssetManager::GetAsset(const Hash nameHash) const {
    // TODO: Reimplement linear search with an O(1) lookup
    for (size_t i = 0; i < this->_AssetCount; i++) {
        if (this->_Assets[i].GetNameHash() == nameHash) {
            return &this->_Assets[i];
        }
    }
    printf("WARNING: Asset not found!");
    return nullptr;
}

void AssetManager::InitializeAssets() {
    // .glsl: No initialization
    // .png: GLTexture
    this->InitializeAssetsWithType<GLTexture>(STRINGHASH(".png"));
    // .pva: SpriteAnimation
    this->InitializeAssetsWithType<SpriteAnimation>(STRINGHASH(".pva"));
    // .pvf: SpriteFont
    this->InitializeAssetsWithType<SpriteFont>(STRINGHASH(".pvf"));
}