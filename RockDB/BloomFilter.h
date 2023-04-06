#include <cstdint>
#include <cstddef>
#include <atomic>
class BloomFilter
{
private:
    inline void AddHash(uint32_t hash);
    template <typename OrFunc>
    inline void AddHash(uint32_t h32, const OrFunc &or_func);
    const uint32_t kNumDoubleProbes;
    uint32_t kLen;
    std::atomic<uint64_t> *data_;
    inline bool DoubleProbe(uint32_t h32, size_t byte_offset) const;
    inline bool MayContainHash(uint32_t h32) const;

public:
    void add(const char *key, int len);
    bool isPresent(const char *key, int len);

    BloomFilter(uint32_t total_bits,
                uint32_t num_probes = 6, size_t huge_page_tlb_size = 0);
};
