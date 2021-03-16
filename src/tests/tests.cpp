
#include <iostream>

#include "collections/BoundedArrayList.h"
#include "memory/Memory.h"

//
// Utilities
//
template <typename T>
void PrintArrayList(const BoundedArrayList<T>& list) {
    std::cout << "[ ";

    constforeach(T, value, list, std::cout << value; std::cout << " "; );

    std::cout << "]" << std::endl;
}

//
// Tests
//
void TestBoundedArrayList() {
    BoundedArrayList<int> list = BoundedArrayList<int>(ThreadAllocator, 10);

    PrintArrayList(list);

    list.Add(69);

    PrintArrayList(list);

    list.Add(420);

    PrintArrayList(list);

    list.Add(5);
    list.Add(10);
    list.Add(3);
    list.Add(6);
    list.Add(7);
    list.Add(8);
    list.Add(9);
    list.Add(10);

    PrintArrayList(list);

    if (list.Add(11)) {
        std::cout << "BAD: Added past end of list\n";
    }
    
    if (list.RemoveFirst(99)) {
        std::cout << "BAD: Removed item that wasn't in list!\n";
    }

    if (!list.RemoveFirst(3)) {
        std::cout << "BAD: Couldn't remove item that was in the list!\n";
    }

    PrintArrayList(list);

    list.Insert(88, 7);

    PrintArrayList(list);

    list[9] = 0;

    PrintArrayList(list);
}

//
// Entrypoint
//
int main(int argc, const char** argv) {

    std::cout << "Testing..." << std::endl;

    TestBoundedArrayList();

    return 0;
}