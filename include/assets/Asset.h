#pragma once

#include <stdint.h>

#include "../Hash.h"

class Allocator;

class Asset {
    friend class AssetManager;
private:
    const Hash _NameHash;
    const Hash _TypeHash;
    size_t _DataLength;
    const void* _Data;
    const void* _Asset;

public:
    Asset(Allocator& allocator, Hash nameHash, Hash typeHash, const void* data, size_t dataLength, const void* asset) : _NameHash(nameHash), _TypeHash(typeHash), _DataLength(dataLength), _Data(data), _Asset(asset) { }

    inline Hash GetNameHash() const { return this->_NameHash; }
    inline Hash GetTypeHash() const { return this->_TypeHash; }
    inline size_t GetDataLength() const { return this->_DataLength; }
    inline const void* GetData() const { return this->_Data; }
    template <typename TAsset>
    inline const TAsset* GetAsset() const { return (TAsset*)this->_Asset; }
};