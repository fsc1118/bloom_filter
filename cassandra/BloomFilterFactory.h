#ifndef BLOONFILTERFACTORY_H
#define BLOONFILTERFACTORY_H

using namespace std;

class BloomFilterFactory
{
private:
    static shared_ptr<BloomFilter> createFilter(int hash, int numElements, int bucketsPer);
public:
    static shared_ptr<BloomFilter> getFilter(long numElements, double maxFalsePosProbability);
    static shared_ptr<BloomFilter> getFilter(long numElements, int targetBucketsPerElem);
};

#endif