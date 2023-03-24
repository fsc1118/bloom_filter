#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

using namespace std;

class BloomFilter
{
private:
    shared_ptr<long[]> indexes(const char *key, int length);
    void setIndexes(long base, long inc, int count, long max, long results[]);

public:
    int hashCount;
    shared_ptr<BitSet> bitSet;
    BloomFilter(int hashCount, shared_ptr<BitSet> bitSet);

    void add(const char *key, int length);
    bool isPresent(const char *key, int length);
};

#endif