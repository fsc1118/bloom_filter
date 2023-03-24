#include <algorithm>
#include <vector>
#include <memory>
#include "BitSet.h"
#include "BloomFilter.h"
#include "BloomCalculator.h"
#include "BloomFilterFactory.h"

#define BITSET_EXCESS 20

shared_ptr<BloomFilter> BloomFilterFactory::getFilter(long numElements, int targetBucketsPerElem)
{
    int maxBucketsPerElement = std::max(1, BloomCalculator::maxBucketsPerElement(numElements));
    int bucketsPerElement = std::min(targetBucketsPerElem, maxBucketsPerElement);
    auto spec = BloomCalculator::computeBloomSpec(bucketsPerElement);
    return BloomFilterFactory::createFilter(spec->K, numElements, spec->bucketsPerElement);
}

shared_ptr<BloomFilter> BloomFilterFactory::getFilter(long numElements, double maxFalsePosProbability)
{
    int bucketsPerElement = BloomCalculator::maxBucketsPerElement(numElements);
    auto spec = BloomCalculator::computeBloomSpec(bucketsPerElement, maxFalsePosProbability);
    return createFilter(spec->K, numElements, spec->bucketsPerElement);
}

shared_ptr<BloomFilter> BloomFilterFactory::createFilter(int hash, int numElements, int bucketsPer)
{
    long numBits = (numElements * bucketsPer) + BITSET_EXCESS;
    return shared_ptr<BloomFilter>(new BloomFilter(hash, shared_ptr<BitSet>(new BitSet(numBits))));
}