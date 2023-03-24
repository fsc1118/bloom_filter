#include <cstddef>
#include <cstdint>
class Hash
{
private:
    inline static uint32_t DecodeFixed32(const char *ptr);

    Hash(){};

public:
    static uint32_t BloomHash(const char *data, size_t n, uint32_t seed);
};