#pragma once

#include <cstdint>
#include <string>

// 64-bit FNV1a Hash

typedef uint64_t Hash;

constexpr uint64_t HashInitialValue = 0xcbf29ce484222325;
constexpr uint64_t HashPrime = 0x100000001b3;

inline const Hash HashData(const void* data, const uint32_t len) {
    const char* bytes = (char*)data;
    uint64_t hash = HashInitialValue;

    for (uint32_t i = 0; i < len; ++i) {
        uint8_t value = bytes[i];
        hash = hash ^ value;
        hash *= HashPrime;
    }

    return hash;
}

inline const Hash HashString(const std::string text) {
    return HashData(text.c_str(), (uint32_t)text.length());
}

// FNV1a C++11 constexpr compile time hash functions, 64 bit
// str should be a null terminated string literal, value should be left out
// e.g HashDataConst("example")
// code license: public domain or equivalent
// post: https://notes.underscorediscovery.com/constexpr-fnv1a/

// constexpr 64-bit FNV1a Hash
inline constexpr uint64_t HashDataConst(const char* const str, const uint64_t value = HashInitialValue, const int iter = 0) noexcept {
    return (str[0] == '\0') ? value : HashDataConst(&str[1], (value ^ uint64_t(str[0])) * HashPrime, iter + 1);
}

/*class Hash {
private:
    const char* _StringData;
    uint64_t _Hash;

public:
    Hash(uint64_t hash);
    Hash(const char* stringData, uint32_t length);

    bool operator==(const Hash& other) const;
    bool operator==(const uint64_t& other) const;
    bool operator!=(const Hash& other) const;
};*/

// STRINGHASH generates a Hash for a literal string, destroying all trace of the literal text.
#define STRINGHASH(str) std::integral_constant<Hash, HashDataConst(str, HashInitialValue, 0)>::value
