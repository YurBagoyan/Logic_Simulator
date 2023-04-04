#ifndef STRINGS_H
#define STRINGS_H

#include <cstdint>

namespace string {

constexpr size_t length(char const *const string)
{
  return *string ? 1 + length(string + 1) : 0;
}

  // fnv1a hash

#if HASH_SIZE == HASH_SIZE_32
using hash_t = uint32_t;
constexpr hash_t FNV_OFFSET{ 0x811c9dc5u };
constexpr hash_t FNV_PRIME{ 0x01000193u };
#elif HASH_SIZE == HASH_SIZE_64
using hash_t = uint64_t;
constexpr hash_t FNV_OFFSET{ 0xcbf29ce484222325 };
constexpr hash_t FNV_PRIME{ 0x100000001b3 };
#endif

constexpr hash_t hash(char const *const key, hash_t const offset = FNV_OFFSET, hash_t const &prime = FNV_PRIME)
{
    hash_t value{ offset };
    size_t const size{ length(key) };
    for (size_t i = 0; i < size; ++i) {
        value ^= static_cast<hash_t>(key[i]);
        value *= prime;
    }

    return value;
}

}


#endif // STRINGS_H
