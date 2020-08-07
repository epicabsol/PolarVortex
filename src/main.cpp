#include <iostream>

#include "memory/Memory.h"
#include "memory/StackAllocator.h"

int main() {
    int* i = (int*)RootAllocator.Allocate(sizeof(i));

    PrintMemoryStats();

    std::cout << "Hello World!";
    return 5;
}