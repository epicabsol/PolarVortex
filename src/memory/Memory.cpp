#include "memory/Memory.h"

#include <assert.h>
#include <stdio.h>

#include "memory/StackAllocator.h"
#include "memory/PoolAllocator.h"

// Disallow default unstance allocation
void* operator new(size_t size) {
    assert(false);
    return nullptr;
}

void operator delete(void* allocation) {
    assert(false);
}

// Disallow default array allocation
void* operator new[](size_t size) {
    assert(false);
    return nullptr;
}

void operator delete[](void* allocation) {
    assert(false);
}

// Root allocator
#define ROOT_ALLOCATOR_SIZE (1024 * 4)
char RootAllocatorBuffer[ROOT_ALLOCATOR_SIZE];
StackAllocator RootAllocator("Root Allocator", RootAllocatorBuffer, sizeof(RootAllocatorBuffer));

// Test pool allocator
#define TEST_POOL_ALLOCATOR_SIZE (sizeof(size_t) * 4)
char TestPoolAllocatorBuffer[TEST_POOL_ALLOCATOR_SIZE];
PoolAllocator<size_t> TestPoolAllocator("Test Pool Allocator", TestPoolAllocatorBuffer, sizeof(TestPoolAllocatorBuffer));

// All allocators
const Allocator* const AllAllocators[] = {
    &RootAllocator,
    &TestPoolAllocator
};
const size_t AllAllocatorsCount = sizeof(AllAllocators) / sizeof(AllAllocators[0]);

void PrintFriendlySize(size_t size) {
    const size_t unit = 1024ull;
    if (size < unit) {
        printf("%4zu B  ", size);
        return;
    }
    else if (size < unit * unit) {
        printf("%4zu KiB", size / unit);
    }
    else if (size < unit * unit * unit) {
        printf("%4zu MiB", size / (unit * unit));
    }
    else {
        printf("%4zu GiB", size / (unit * unit * unit));
    }
}

void PrintAllocatorStats(const Allocator* allocator) {
    printf("  %20s: [", allocator->GetName());

    const size_t totalWidth = 10;

    size_t width = allocator->GetFree() * totalWidth / allocator->GetCapacity();
    for (size_t i = 0; i < (totalWidth - width); i++) {
        printf("=");
    }
    for (size_t i = 0; i < width; i++) {
        printf(" ");
    }

    printf("] ");

    PrintFriendlySize(allocator->GetFree());
    printf(" free of ");
    PrintFriendlySize(allocator->GetCapacity());
    printf(" (%2.1f%%)\n", (float)allocator->GetFree() * 100.0f / (float)allocator->GetCapacity());
}

void PrintMemoryStats() {
    printf("Memory Allocators[%zu]:\n", AllAllocatorsCount);
    for (size_t i = 0; i < AllAllocatorsCount; i++) {
        PrintAllocatorStats(AllAllocators[i]);
    }
}