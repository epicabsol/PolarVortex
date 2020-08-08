#pragma once

#include "../Hash.h"
#include "../memory/StackAllocator.h"

class Asset;

class AssetManager {
private:
    Asset* _Assets;
    size_t _AssetCount;

public:
    AssetManager();

    Asset* GetAsset(const Hash nameHash) const;
};

extern AssetManager* Assets;