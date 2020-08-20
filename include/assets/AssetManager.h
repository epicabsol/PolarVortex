#pragma once

#include "Asset.h"
#include "../Hash.h"
#include "../game/PolarVortexGame.h"
#include "../memory/Memory.h"
#include "../memory/StackAllocator.h"

class AssetManager {
private:
    Allocator& _Allocator;
    Asset* _Assets;
    size_t _AssetCount;

    
    /**
     * @brief Initializes a new instance of T for each of the assets with the given type hash.
     * 
     * @tparam T The type of instance to create from the matching assets' data.
     * @param typeHash The type hash which is used to determine whether to initialize each asset.
     */
    template <typename T>
    void InitializeAssetsWithType(Hash typeHash) {
        for (size_t i = 0; i < this->_AssetCount; i++) {
            if (this->_Assets[i].GetTypeHash() == typeHash) {
                this->_Assets[i]._Asset = AssetsAllocator.New<T>((const char*)this->_Assets[i].GetData(), this->_Assets[i].GetDataLength());
            }
        }
    }

public:
    AssetManager(Allocator& allocator);

    Asset* GetAsset(const Hash nameHash) const;

    /**
     * @brief Initializes the native types for assets that were read in the constructor.
     * 
     * Depends on the game's renderer existing.
     */
    void InitializeAssets();
};
