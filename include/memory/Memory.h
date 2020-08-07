#pragma once

#include <stdint.h>

class Allocator;
class StackAllocator;

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

// Global allocators
extern StackAllocator RootAllocator;
extern const Allocator* const AllAllocators[];
extern const size_t AllAllocatorsCount;

/**
 * @brief Prints statistics about memory allocation to standard output.
 */
void PrintMemoryStats();