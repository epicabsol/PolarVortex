#include <iostream>

#include "memory/Memory.h"
#include "memory/StackAllocator.h"
#include "memory/PoolAllocator.h"

int main() {
    const void* top = RootAllocator.GetTop();
    int* i = (int*)RootAllocator.Allocate(sizeof(i));
    size_t* s_t = (size_t*)TestPoolAllocator.Allocate(sizeof(size_t));

    PrintMemoryStats();

    TestPoolAllocator.Free(s_t);
    RootAllocator.Pop(top);

    PrintMemoryStats();

    std::cout << "Hello World!";
    return 5;
}