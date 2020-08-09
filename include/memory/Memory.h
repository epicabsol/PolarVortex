#pragma once

#include <assert.h>
#include <stdint.h>

#include "StackAllocator.h"
#include "PoolAllocator.h"

/**
 * @brief Aligns the given unaligned value to the given alignment.
 *
 * @param unaligned The unaligned value to be aligned.
 * @param alignment The alignment for the result. Must be a power of two.
 * @return The aligned value.
 */
inline size_t Align(size_t unaligned, size_t alignment) {
    // Thanks to https://stackoverflow.com/a/32309869
    assert((alignment & 1) == 0);
    assert(alignment > 1);
    return (unaligned + (alignment - 1)) & ~(alignment - 1);
}

//
// Override the default dynamic allocation system
//
// Inspired by http://www.swedishcoding.com/2008/08/31/are-we-out-of-memory/
//

// Instance allocation
void* operator new(size_t size);
void operator delete(void* allocation);

// Array allocation
void* operator new[](size_t size);
void operator delete[](void* allocation);

void* operator new(size_t size, Allocator& allocator);
void operator delete(void* allocation, Allocator& allocator);

// Global allocators
extern StackAllocator RootAllocator;
extern StackAllocator AssetsAllocator;
extern StackAllocator FrameAllocator;
extern const Allocator* const AllAllocators[];
extern const size_t AllAllocatorsCount;

/**
 * @brief Prints statistics about memory allocation to standard output.
 */
void PrintMemoryStats();