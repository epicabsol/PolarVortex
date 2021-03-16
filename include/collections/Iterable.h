#pragma once

#include "Iterator.h"

#define foreach_c(itemtype, constness, item, collection, code) do { \
    constness Iterator<itemtype>* it = collection.CreateIterator(ThreadAllocator); \
    while (it->HasNext()) { \
        constness itemtype& item = it->Next(); \
        code \
    } \
    ThreadAllocator.Delete(it); \
} while (false)

#define foreach(itemtype, item, collection, code) foreach_c(itemtype, , item, collection, code)
#define constforeach(itemtype, item, collection, code) foreach_c(itemtype, const, item, collection, code)

class Allocator;

template <typename T>
class Iterable {
public:
    virtual Iterator<T>* CreateIterator(Allocator& allocator) = 0;
    virtual const Iterator<T>* CreateIterator(Allocator& allocator) const = 0;
};