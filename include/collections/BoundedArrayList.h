#pragma once

#include <stddef.h>
#include <utility>

#include "Iterator.h"
#include "Iterable.h"
#include "../memory/Allocator.h"

/**
 * @brief A contiguous collection of elements of type T.
 * The maximum capacity and allocator used to store the elements
 * is fixed.
 * 
 * @tparam T The type of elements in this BoundedArrayList.
 */
template <typename T>
class BoundedArrayList : public Iterable<T> {
private:
    T* _Buffer;
    size_t _Capacity;
    size_t _Count;
    Allocator& _Allocator;

    class Iterator : public ::Iterator<T> {
    private:
        BoundedArrayList<T>* _List;
        size_t _NextIndex;
        Allocator& _Allocator;

    public:
        Iterator(Allocator& allocator, BoundedArrayList<T>* list) : _Allocator(allocator), _List(list), _NextIndex(0) { }

        virtual bool HasNext() const override { return this->_NextIndex < this->_List->_Count; }
        virtual T& Next() override { return (*this->_List)[this->_NextIndex++]; }
        virtual const T& Next() const override { return (*this->_List)[((BoundedArrayList<T>::Iterator*)this)->_NextIndex++]; }
    };

    inline void ShiftElements(size_t startIndex, size_t count, int amount) {
        // Assert that we aren't shifting out-of-bounds
        if (startIndex + count + amount > this->_Capacity || startIndex + amount < 0) {
            // Oh no! Out of bounds!
            return;
        }
        if (amount > 0) {
            for (size_t i = startIndex + count + amount - 1; i >= startIndex + amount; i--) {
                this->_Buffer[i] = std::move(this->_Buffer[i - amount]);
            }
        }
        else if (amount < 0) {
            for (size_t i = startIndex + amount; i < startIndex + count + amount; i++) {
                this->_Buffer[i] = std::move(this->_Buffer[i - amount]);
            }
        }
    }

public:
    BoundedArrayList(Allocator& allocator, size_t capacity) : _Allocator(allocator), _Capacity(capacity), _Count(0), _Buffer(nullptr) {
        this->_Buffer = (T*)allocator.Allocate(sizeof(T) * capacity);
    }

    ~BoundedArrayList() {
        for (size_t i = 0; i < this->_Count; i++) {
            this->_Buffer[i].~T();
        }
        this->_Allocator.Free(this->_Buffer);
    }

    inline size_t GetCount() const { return this->_Count; }
    inline size_t GetCapacity() const { return this->_Capacity; }

    bool Add(T&& value) {
        if (this->_Count >= this->_Capacity) {
            return false;
        }
        else {
            this->_Buffer[this->_Count] = value;
            this->_Count++;
            return true;
        }
    }

    bool Add(const T& value) {
        if (this->_Count >= this->_Capacity) {
            return false;
        }
        else {
            this->_Buffer[this->_Count] = value;
            this->_Count++;
            return true;
        }
    }

    bool Insert(T&& value, size_t index) {
        if (this->_Count >= this->_Capacity || index > this->_Count) {
            return false;
        }
        else {
            this->ShiftElements(index, this->_Count - index, 1);
            this->_Buffer[index] = value;
            this->_Count++;
            return true;
        }
    }

    bool Insert(const T& value, size_t index) {
        if (this->_Count >= this->_Capacity || index > this->_Count) {
            return false;
        }
        else {
            this->ShiftElements(index, this->_Count - index, 1);
            this->_Buffer[index] = value;
            this->_Count++;
        }
    }

    bool RemoveFirst(const T& value) {
        for (size_t i = 0; i < this->_Count; i++) {
            if (this->_Buffer[i] == value) {
                this->RemoveAt(i);
                return true;
            }
        }
        return false;
    }

    void RemoveAt(size_t index) {
        this->_Buffer[index].~T();
        this->ShiftElements(index + 1, this->_Count - index - 1, -1);
        this->_Count--;
    }

    // Iterable
    virtual ::Iterator<T>* CreateIterator(Allocator& allocator) override { return allocator.New<Iterator>(this); }
    virtual const ::Iterator<T>* CreateIterator(Allocator& allocator) const override { return allocator.New<Iterator>((BoundedArrayList<T>*)this); }

    // Operators
    T& operator[](size_t index) {
        return this->_Buffer[index];
    }

    const T& operator[](size_t index) const {
        return this->_Buffer[index];
    }
};