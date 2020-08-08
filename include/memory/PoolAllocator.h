#pragma once

#include <assert.h>
#include <stdint.h>

#include "Allocator.h"
#include "Memory.h"

/**
 * @brief Allocates memory from a reuseable pool of instances.
 *
 * @tparam T The type of element that is being pooled.
 */
template <typename T>
class PoolAllocator : public Allocator {
private:
    union Element {
        T Occupied;
        struct {
            Element* NextFree;
        } Vacant;
    };

    Element* _FirstFree;
    Element* _Elements;
    const size_t _Alignment;
    size_t _Capacity;
    size_t _FreeCount;

public:
    PoolAllocator(const char* name, void* buffer, size_t bufferLength, size_t alignment = alignof(T)) : Allocator(name, buffer, bufferLength), _FirstFree(nullptr), _Elements(nullptr), _Alignment(alignment), _Capacity(0), _FreeCount(0) {
        this->_Elements = (Element*)Align((size_t)buffer, alignment);
        this->_FirstFree = this->_Elements;
        this->_Capacity = ((size_t)buffer + bufferLength - (size_t)this->_Elements) / sizeof(Element);
        this->_FreeCount = this->_Capacity;

        for (size_t i = 0; i < this->_Capacity - 1; i++) {
            this->_Elements[i].Vacant.NextFree = &this->_Elements[i + 1];
        }
        this->_Elements[this->_Capacity - 1].Vacant.NextFree = nullptr;
    }

    virtual size_t GetFree() const override { return this->_FreeCount * sizeof(Element); }

    virtual void* Allocate(size_t length) override {
        assert(length == sizeof(T));
        assert(this->_FirstFree != nullptr);

        void* result = this->_FirstFree;
        this->_FirstFree = this->_FirstFree->Vacant.NextFree;
        this->_FreeCount -= 1;
        return result;
    }

    virtual void Free(void* allocation) override {
        assert((size_t)allocation >= (size_t)this->_Elements);
        assert((size_t)allocation < (size_t)&this->_Elements[this->_Capacity]);

        Element* element = (Element*)allocation;
        element->Vacant.NextFree = this->_FirstFree;
        this->_FirstFree = element;
        this->_FreeCount += 1;
    }
};