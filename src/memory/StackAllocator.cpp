#include "memory/StackAllocator.h"

#include <assert.h>

void* StackAllocator::Allocate(size_t length) {
    void* result = this->_Top;
    this->_Top = (void*)((char*)this->_Top + length);

    // Assert that there is enough space left
    assert((size_t)this->_Top <= (size_t)this->_Buffer + this->_BufferLength);

    return result;
}

void StackAllocator::Free(void* allocation) {
    // Stack allocators can't free individual allocations - record the top with GetTop() and then later use that pointer with Pop().
    assert(false);
}

void StackAllocator::Pop(const void* newTop) {
    // Assert we aren't popping past the bottom of the buffer
    assert((size_t)newTop >= (size_t)this->_Buffer);

    // Assert we aren't popping farther into the buffer past the current top
    assert((size_t)newTop <= (size_t)this->_Top);

    this->_Top = (void*)newTop;
}