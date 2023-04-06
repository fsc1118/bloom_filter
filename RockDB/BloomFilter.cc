#include <assert.h>
#include <algorithm>
#include <memory.h>
#include <stdlib.h>
#include "BloomFilter.h"
#include "Murmurhash.h"
#include "FastRange32.h"

#define PREFETCH(addr, rw, locality) __builtin_prefetch(addr, rw, locality)

static uint32_t roundUpToPow2(uint32_t x)
{
    uint32_t rv = 1;
    while (rv < x)
    {
        rv <<= 1;
    }
    return rv;
}

template <typename OrFunc>
inline void BloomFilter::AddHash(uint32_t h32, const OrFunc &or_func)
{
    size_t a = FastRange32(kLen, h32);
    PREFETCH(data_ + a, 0, 3);
    // Expand/remix with 64-bit golden ratio
    uint64_t h = 0x9e3779b97f4a7c13ULL * h32;
    for (unsigned i = 0;; ++i)
    {
        // Two bit probes per uint64_t probe
        uint64_t mask =
            ((uint64_t)1 << (h & 63)) | ((uint64_t)1 << ((h >> 6) & 63));
        or_func(&data_[a ^ i], mask);
        if (i + 1 >= kNumDoubleProbes)
        {
            return;
        }
        h = (h >> 12) | (h << 52);
    }
}

void BloomFilter::AddHash(uint32_t hash)
{
    AddHash(hash, [](std::atomic<uint64_t> *ptr, uint64_t mask)
            { ptr->store(ptr->load(std::memory_order_relaxed) | mask,
                         std::memory_order_relaxed); });
}

void BloomFilter::add(const char *key, int len)
{
    long *indexes = new long[2];
    MurmurHash::hash(key, 0, len, 0, indexes);
    AddHash(indexes[0]);
}

bool BloomFilter::isPresent(const char *key, int len)
{
    long *indexes = new long[2];
    MurmurHash::hash(key, 0, len, 0, indexes);
    return (MayContainHash(indexes[0]));
}

BloomFilter::BloomFilter(uint32_t total_bits,
                         uint32_t num_probes, size_t huge_page_tlb_size)
    // Round down, except round up with 1
    : kNumDoubleProbes((num_probes + (num_probes == 1)) / 2)
{
    assert(num_probes % 2 == 0); // limitation of current implementation
    assert(num_probes <= 10);    // limitation of current implementation
    assert(kNumDoubleProbes > 0);

    // Determine how much to round off + align by so that x ^ i (that's xor) is
    // a valid u64 index if x is a valid u64 index and 0 <= i < kNumDoubleProbes.
    uint32_t block_bytes = /*bytes/u64*/ 8 *
                           /*u64s*/ std::max(1U, roundUpToPow2(kNumDoubleProbes));
    uint32_t block_bits = block_bytes * 8;
    uint32_t blocks = (total_bits + block_bits - 1) / block_bits;
    uint32_t sz = blocks * block_bytes;
    kLen = sz / /*bytes/u64*/ 8;
    assert(kLen > 0);
    sz += block_bytes - 1;
    // Assume malloc returns aligned memory
    char *raw = (char *)malloc(sz);
    memset(raw, 0, sz);
    auto block_offset = reinterpret_cast<uintptr_t>(raw) % block_bytes;
    if (block_offset > 0)
    {
        // Align on block_bytes boundary
        raw += block_bytes - block_offset;
    }
    data_ = reinterpret_cast<std::atomic<uint64_t> *>(raw);
}

inline bool BloomFilter::MayContainHash(uint32_t h32) const
{
    size_t a = FastRange32(kLen, h32);
    PREFETCH(data_ + a, 0, 3);
    return DoubleProbe(h32, a);
}

inline bool BloomFilter::DoubleProbe(uint32_t h32, size_t byte_offset) const
{
    // Expand/remix with 64-bit golden ratio
    uint64_t h = 0x9e3779b97f4a7c13ULL * h32;
    for (unsigned i = 0;; ++i)
    {
        // Two bit probes per uint64_t probe
        uint64_t mask =
            ((uint64_t)1 << (h & 63)) | ((uint64_t)1 << ((h >> 6) & 63));
        uint64_t val = data_[byte_offset ^ i].load(std::memory_order_relaxed);
        if (i + 1 >= kNumDoubleProbes)
        {
            return (val & mask) == mask;
        }
        else if ((val & mask) != mask)
        {
            return false;
        }
        h = (h >> 12) | (h << 52);
    }
}
