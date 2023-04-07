#include <algorithm>
#include <memory>
#include <vector>
#include "BloomFilter.h"
#include "BloomFilterFactory.h"

#define K_BENCHMARK 6
#define SIZE_BENCHMARK ((1 << 17) << 9)

shared_ptr<BloomFilter> BloomFilterFactory::createFilter_benchmark() {
  return shared_ptr<BloomFilter>(new BloomFilter(6, SIZE_BENCHMARK));
}
