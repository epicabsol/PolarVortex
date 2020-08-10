#include "memory/Memory.h"

#include <assert.h>
#include <stdio.h>

#include "memory/StackAllocator.h"
#include "memory/PoolAllocator.h"

// Redirect default new and delete to the frame allocator
void* operator new(size_t size) {
    return FrameAllocator.Allocate(size);
}

void operator delete(void* allocation) {
    FrameAllocator.Free(allocation);
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
#define ROOT_ALLOCATOR_SIZE (1024 * 1024 * 8)
char RootAllocatorBuffer[ROOT_ALLOCATOR_SIZE];
StackAllocator RootAllocator("Root", RootAllocatorBuffer, sizeof(RootAllocatorBuffer));

// Assets allocator
#define ASSETS_ALLOCATOR_SIZE (1024 * 1024 * 64)
char AssetsAllocatorBuffer[ASSETS_ALLOCATOR_SIZE];
StackAllocator AssetsAllocator("Assets", AssetsAllocatorBuffer, sizeof(AssetsAllocatorBuffer));

// Screen allocator
#define SCREEN_ALLOCATOR_SIZE (1024 * 1024 * 16)
char ScreenAllocatorBuffer[SCREEN_ALLOCATOR_SIZE];
StackAllocator ScreenAllocator("Screen", ScreenAllocatorBuffer, sizeof(ScreenAllocatorBuffer));

// Frame allocator
#define FRAME_ALLOCATOR_SIZE (1024 * 16)
char FrameAllocatorBuffer[FRAME_ALLOCATOR_SIZE];
StackAllocator FrameAllocator("Frame", FrameAllocatorBuffer, sizeof(FrameAllocatorBuffer));

// All allocators
const Allocator* const AllAllocators[] = {
    &RootAllocator,
    &AssetsAllocator,
    &ScreenAllocator,
    &FrameAllocator
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

    const size_t totalWidth = 24;

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
    printf(" (%2.1f%% free)\n", (float)allocator->GetFree() * 100.0f / (float)allocator->GetCapacity());
}

void PrintMemoryStats() {
    printf("Memory Allocators[%zu]:\n", AllAllocatorsCount);
    for (size_t i = 0; i < AllAllocatorsCount; i++) {
        PrintAllocatorStats(AllAllocators[i]);
    }
}