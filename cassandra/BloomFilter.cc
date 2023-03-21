#include <memory>
#include "BitSet.h"
#include "BloomFilter.h"
#include "util.h"
#include "Murmurhash.h"
BloomFilter::BloomFilter(int hashCount, shared_ptr<BitSet> bitSet) {
    this->hashCount = hashCount;
    this->bitSet = bitSet;
}

void BloomFilter::setIndexes(long base, long inc, int count, long max, long results[]) {
    for (int i = 0; i < count; i++)
    {
        results[i] = abstract_val(base % max);
        base += inc;
    }
}

shared_ptr<long[]> BloomFilter::indexes(const char *key, int length) {
    long* indexes = new long[21];
    MurmurHash::hash(key, 0, length, 0, indexes);
    setIndexes(indexes[1], indexes[0], hashCount, bitSet->capacity(), indexes);
    return shared_ptr<long[]>(indexes);
}

void BloomFilter::add(const char *key, int length) {
    auto idx = indexes(key, length);
    for (int i = 0; i < hashCount; i++)
    {
        bitSet->set(idx[i]);
    }
}

bool BloomFilter::isPresent(const char *key, int length) {
    auto idx = indexes(key, length);
    for (int i = 0; i < hashCount; i++)
    {
        if (!(bitSet->get(idx[i])))
        {
            return false;
        }
    }
    return true;
}