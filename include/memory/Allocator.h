#pragma once

#include <stdint.h>

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
    Allocator(const char* name, void* buffer, size_t bufferLength) : _Name(name), _Buffer(buffer), _BufferLength(bufferLength) { }

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
    size_t GetCapacity() const { return this->_BufferLength; }

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
     * @brief Frees the given allocation that was allocated from this Allocator.
     *
     * @param allocation The address of the bytes to deallocate.
     */
    virtual void Free(void* allocation) = 0;
};