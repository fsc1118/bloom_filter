#ifndef FASTRANGE32_H
#define FASTRANGE32_H

#include <cstdint>
#include <cstddef>
#include <type_traits>
template <typename Hash, typename Range>
struct FastRangeGenericImpl
{
    // only reach this on no supported specialization
};

template <typename Range>
struct FastRangeGenericImpl<uint32_t, Range>
{
    static inline Range Fn(uint32_t hash, Range range)
    {
        static_assert(std::is_unsigned<Range>::value, "must be unsigned");
        static_assert(sizeof(Range) <= sizeof(uint32_t),
                      "cannot be larger than hash (32 bits)");

        uint64_t product = uint64_t{range} * hash;
        return static_cast<Range>(product >> 32);
    }
};

template <typename Hash, typename Range>
inline Range FastRangeGeneric(Hash hash, Range range)
{
    return FastRangeGenericImpl<Hash, Range>::Fn(hash, range);
}

inline uint32_t FastRange32(uint32_t hash, uint32_t range)
{
    return FastRangeGeneric(hash, range);
}

#endif