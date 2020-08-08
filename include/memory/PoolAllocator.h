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
    char* _Elements;
    const size_t _Alignment;
    size_t _Capacity;
    size_t _FreeCount;
    size_t _ElementLength;

public:
    /**
     * @brief Constructs a new PoolAllocator with the given name that allocates instances with the given alignment from the given buffer.
     *
     * @param name The name for debugging purposes. Does not copy the value, so the given pointer must stay valid.
     * @param buffer The buffer to allocate from.
     * @param bufferLength The length of `buffer`.
     * @param alignment The alignment of the addresses of the individual elements in the pool.
     */
    PoolAllocator(const char* name, void* buffer, size_t bufferLength, size_t alignment = alignof(T)) : Allocator(name, buffer, bufferLength), _FirstFree(nullptr), _Elements(nullptr), _Alignment(alignment), _Capacity(0), _FreeCount(0), _ElementLength(Align(sizeof(Element), alignment)) {
        assert(alignment > 0);

        this->_Elements = (char*)Align((size_t)buffer, alignment);
        this->_FirstFree = (Element*)this->_Elements;
        this->_Capacity = ((size_t)buffer + bufferLength - (size_t)this->_Elements) / this->_ElementLength;
        this->_FreeCount = this->_Capacity;

        for (size_t i = 0; i < this->_Capacity - 1; i++) {
            ((Element*)&this->_Elements[i * this->_ElementLength])->Vacant.NextFree = (Element*)&this->_Elements[(i + 1) * this->_ElementLength];
        }
        ((Element*)&this->_Elements[(this->_Capacity - 1) * this->_ElementLength])->Vacant.NextFree = nullptr;
    }

    virtual size_t GetFree() const override { return this->_FreeCount * this->_ElementLength; }

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
        assert((size_t)allocation < (size_t)this->_Elements + this->_Capacity * this->_ElementLength);

        Element* element = (Element*)allocation;
        element->Vacant.NextFree = this->_FirstFree;
        this->_FirstFree = element;
        this->_FreeCount += 1;
    }
};