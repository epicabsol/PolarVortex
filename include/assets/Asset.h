#pragma once

#include <stdint.h>

#include "../Hash.h"

class Asset {
private:
    const Hash _NameHash;
    size_t _DataLength;
    const void* _Data;

public:
    Asset(Hash nameHash, const void* data, size_t dataLength) : _NameHash(nameHash), _DataLength(dataLength), _Data(data) { }

    Hash GetNameHash() const { return this->_NameHash; }
    size_t GetDataLength() const { return this->_DataLength; }
    const void* GetData() const { return this->_Data; }
};