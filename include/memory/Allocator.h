#pragma once

#include <stdint.h>
#include <assert.h>
#include <stdio.h>

/**
 * @brief Defines an abstract base class for memory allocators.
 *
 * At first you're probably thinking, 'wait - do we really want to make every memory allocation a virtual function invocation?'
 * My answer is, 'yes', because it's a nice design, facilitates debug instrumentaiton, and our goal is already to dynamically allocate as few times as possible.
 */
class Allocator {
private:
    const char* _Name;

protected:
    void* const _Buffer;
    const size_t _BufferLength;

public:
    /**
     * @brief Constructs a new Allocator object with the given name that allocates memory from the given buffer.
     *
     * @param name The name for debugging purposes. Does not copy the value, so the given pointer must stay valid.
     * @param buffer The buffer to allocate from.
     * @param bufferLength The length of `buffer`.
     */
    inline Allocator(const char* name, void* buffer, size_t bufferLength) : _Name(name), _Buffer(buffer), _BufferLength(bufferLength) { }

    /**
     * @brief Gets the name that was assigned at construction.
     *
     * @return The name of this Allocator.
     */
    inline const char* GetName() const { return this->_Name; }

    /**
     * @brief Gets the total number of bytes that can be allocated from this Allocator, including any overhead.
     *
     * @return The capacity of this Allocator.
     */
    inline size_t GetCapacity() const { return this->_BufferLength; }

    /**
     * @brief Gets the number of remaining bytes that can be allocated from this Allocator, including any overhead.
     *
     * @return The number of free bytes in this Allocator.
     */
    virtual size_t GetFree() const = 0;

    /**
     * @brief Allocates the given number of contiguous bytes.
     *
     * @param length The number of bytes to allocate.
     * @return The address of the bytes that were allocated, or `nullptr` if the allocation failed.
     */
    virtual void* Allocate(size_t length) = 0;

    /**
     * @brief Creates a new instance of type `T`.
     * 
     * Passes in `this` as the first argument to the constructor, and then passes in the rest of the given arguments.
     *
     * @tparam T The type to instantiate.
     * @tparam TArgs The types of the arguments to pass to the instance's constructor.
     * @param args The additional arguments to pass to the instance's constructor.
     * @return The allocated and initialized instance.
     */
    template <typename T, typename ... TArgs>
    inline T* New(TArgs ... args) {
        T* address = (T*)this->Allocate(sizeof(T));
        T* result = address;
        if (address != nullptr) {
            result = new (address) T(*this, args...);
            assert(address == result); // Can't have implementations returning weird address in placement new!
        }
        else {
            printf("ERROR: Couldn't allocator memory from allocator %s!\n", this->_Name);
        }

        return result;
    }

    /**
     * @brief Frees the given allocation that was allocated from this Allocator.
     *
     * @param allocation The address of the bytes to deallocate.
     */
    virtual void Free(void* allocation) = 0;

    /**
     * @brief Deletes the given instance.
     *
     * @tparam T The type of the instance to delete.
     * @param instance An instance of type `T` that was allocated from this Allocator.
     */
    template <typename T>
    inline void Delete(T* instance) {
        instance->~T();
        this->Free((void*)instance);
    }
};