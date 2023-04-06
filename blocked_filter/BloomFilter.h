#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

using namespace std;

class BloomFilter
{
public:
    int hashCount;
    char* bitSet;
    BloomFilter(int hashCount, long numBits);

    void add(const char *key, int length);
    bool isPresent(const char *key, int length);
};

#endif
