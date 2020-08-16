#pragma once

#include "../Hash.h"
#include "../memory/StackAllocator.h"

class Asset;

class AssetManager {
private:
    Allocator& _Allocator;
    Asset* _Assets;
    size_t _AssetCount;

public:
    AssetManager(Allocator& allocator);

    Asset* GetAsset(const Hash nameHash) const;
};
