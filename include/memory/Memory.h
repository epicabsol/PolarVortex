#pragma once

#include <assert.h>
#include <stdint.h>

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

#include "StackAllocator.h"
#include "PoolAllocator.h"

//
// Override the default dynamic allocation system
//
// Inspired by http://www.swedishcoding.com/2008/08/31/are-we-out-of-memory/
//

// Instance allocation
/*void* operator new(size_t size) noexcept(false);
void operator delete(void* allocation) noexcept(true);

// Array allocation
void* operator new[](size_t size) noexcept(false);
void operator delete[](void* allocation) noexcept(true);*/

void* operator new(size_t size, Allocator& allocator) noexcept(false);
void operator delete(void* allocation, Allocator& allocator) noexcept(true);

// Global allocators
extern StackAllocator RootAllocator;
extern StackAllocator AssetsAllocator;
extern StackAllocator ScreenAllocator;
extern StackAllocator FrameAllocator;
extern thread_local StackAllocator ThreadAllocator;
extern const Allocator* const AllAllocators[];
extern const size_t AllAllocatorsCount;

/**
 * @brief Prints statistics about memory allocation to standard output.
 */
void PrintMemoryStats();