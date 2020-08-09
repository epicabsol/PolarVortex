#pragma once

#include <stdint.h>

#include "Allocator.h"

/**
 * @brief Allocates memory linearly, and allows popping off the most recent allocations.
 *
 * Usage: Allocate like usual.
 * Store the top at some point using the `GetTop()` method, and then later deallocate everything since then by passing the result to `Pop()`.
 */
class StackAllocator : public Allocator {
private:
    void* _Top;

public:
    /**
     * @brief Constructs a new StackAllocator object with the given name that allocates memory from the given buffer.
     *
     * @param name The name for debugging purposes. Does not copy the value, so the given pointer must stay valid.
     * @param buffer The buffer to allocate from.
     * @param bufferLength The length of `buffer`.
     */
    inline StackAllocator(const char* name, void* buffer, size_t bufferLength) : Allocator(name, buffer, bufferLength), _Top(buffer) { }

    /**
     * @brief Gets the address of the top of the stack, to be popped to later.
     *
     * @return The top of the allocation stack.
     */
    inline const void* GetTop() const { return this->_Top; }

    virtual size_t GetFree() const override { return this->_BufferLength - ((size_t)this->_Top - (size_t)this->_Buffer); }

    virtual void* Allocate(size_t length) override;

    virtual void Free(void* allocation) override;

    /**
     * @brief Frees allocations until the given address is once again the top of the allocation stack.
     *
     * @param newTop The address of the stack to pop back to.
     */
    void Pop(const void* newTop);
};