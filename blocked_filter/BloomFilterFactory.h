#ifndef BLOONFILTERFACTORY_H
#define BLOONFILTERFACTORY_H

using namespace std;

class BloomFilterFactory
{
public:
    static shared_ptr<BloomFilter> createFilter_benchmark();
};

#endif
