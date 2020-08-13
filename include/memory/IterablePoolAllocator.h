#pragma once

#include <iterator>

#include "PoolAllocator.h"

/**
 * @brief A pool allocator that keeps track of which elements have been allocated, so it can perform a cache-friendly iteration through them.
 *
 * @tparam T The type of element that is being pooled.
 */
template <typename T>
class IterablePoolAllocator : public PoolAllocator<T> {
private:
    uint8_t* _OccupiedBuffer;

    typedef Element PoolElement;
    inline char* GetElements() { return this->_Elements; }
    inline size_t FindNextOccupied(size_t startIndex) {
        while (startIndex < this->_Capacity) {
            startIndex += 1;

            size_t obIndex = startIndex / (sizeof(uint8_t) * 8);
            size_t obBit = startIndex % (sizeof(uint8_t) * 8);
            if (this->_OccupiedBuffer[obIndex] & (1 << obBit)) {
                break;
            }
        }
        return startIndex;
    }

public:
    class Iterator {
    private:
        IterablePoolAllocator<T>* _Allocator;
        size_t _Index;

    public:
        Iterator(IterablePoolAllocator<T>* allocator, size_t index = 0) : _Allocator(allocator), _Index(index) {
            // If we were given an empty index before the end, find the next occupied index.
            if (index < this->_Allocator->_Capacity && ((this->_Allocator->_OccupiedBuffer[0] & 1) == 0)) {
                this->_Index = this->_Allocator->FindNextOccupied(index);
            }
        }

        Iterator& operator++() {
            // Linear search for the next occupied element, starting at _Index + 1
            this->_Index = this->_Allocator->FindNextOccupied(this->_Index);
            return *this;
        }

        Iterator operator++(int) { Iterator result = *this; ++(*this); return result; } // ?????
        bool operator==(const Iterator& other) const { return this->_Allocator == other._Allocator && this->_Index == other._Index; }
        bool operator!=(const Iterator& other) const { return !(*this == other); }
        T& operator*() { return (((PoolElement*)this->_Allocator->GetElements())[this->_Index]).Occupied; }

        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::input_iterator_tag;
    };

    IterablePoolAllocator(const char* name, void* buffer, size_t bufferLength, size_t alignment = alignof(T)) : PoolAllocator(name, buffer, bufferLength - ((bufferLength / Align(sizeof(Element), alignment)) / (sizeof(uint8_t) * 8) + 1), alignment), _OccupiedBuffer((uint8_t*)buffer + this->_BufferLength) {
        for (size_t i = 0; i < this->_Capacity; i++) {
            this->_OccupiedBuffer[i] = 0;
        }
    }

    virtual void* Allocate(size_t length) override {
        void* result = PoolAllocator<T>::Allocate(length);

        // Mark as occupied in the occupied buffer
        size_t index = ((size_t)result - (size_t)this->_Elements) / this->_ElementLength;
        size_t obIndex = index / (sizeof(uint8_t) * 8);
        size_t obBit = index % (sizeof(uint8_t) * 8);
        this->_OccupiedBuffer[obIndex] |= (1 << obBit);

        return result;
    }

    virtual void Free(void* allocation) override {
        PoolAllocator<T>::Free(allocation);

        // Mark as free in the occupied buffer
        size_t index = ((size_t)allocation - (size_t)this->_Elements) / this->_ElementLength;
        size_t obIndex = index / (sizeof(uint8_t) * 8);
        size_t obBit = index % (sizeof(uint8_t) * 8);
        this->_OccupiedBuffer[obIndex] &= ~(1 << obBit);
    }

    Iterator begin() {
        return Iterator(this, 0);
    }

    Iterator end() {
        return Iterator(this, this->_Capacity);
    }
};