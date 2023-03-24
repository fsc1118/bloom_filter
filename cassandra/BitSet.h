#ifndef BITSET_H
#define BITSET_H

class BitSet
{
private:
    long size;
    char *bytes;

public:
    BitSet(long numBits);
    bool get(long index);
    void set(long index);
    void clear();
    long capacity();
};

#endif
