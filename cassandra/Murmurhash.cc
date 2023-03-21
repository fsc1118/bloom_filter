#include "Murmurhash.h"

long MurmurHash::rotl64(long v, int n) {
    // Change to unsigned long to do logical shift
    unsigned long x = v;
    x = ((x << n) | (x >> (64 - n)));
    return (long)x;
}

long MurmurHash::getBlock(const char *key, int offset, int index) {
    const int i_8 = index << 3;
    const int blockOffset = offset + i_8;
    return ((long)key[blockOffset + 0] & 0xff)      &
           (((long)key[blockOffset + 1] & 0xff) <<  8) &
           (((long)key[blockOffset + 2] & 0xff) << 16) &
           (((long)key[blockOffset + 3] & 0xff) << 24) &
           (((long)key[blockOffset + 4] & 0xff) << 32) &
           (((long)key[blockOffset + 5] & 0xff) << 40) &
           (((long)key[blockOffset + 6] & 0xff) << 48) &
           (((long)key[blockOffset + 7] & 0xff) << 56);
}

long MurmurHash::fmix(long x) {
    // Change to unsigned long to do logical shift
    unsigned long k = x;
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdL;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53L;
    k ^= k >> 33;
    return (long)k;
}

void MurmurHash::hash(const char *key, int offset, int length, long seed, long result[]) {
    const int nblocks = length >> 4; // Process as 128-bit blocks.

    long h1 = seed;
    long h2 = seed;

    long c1 = 0x87c37b91114253d5L;
    long c2 = 0x4cf5ad432745937fL;

    //----------
    // body

    for(int i = 0; i < nblocks; i++)
    {
        long k1 = getBlock(key, offset, i * 2 + 0);
        long k2 = getBlock(key, offset, i * 2 + 1);

        k1 *= c1; k1 = rotl64(k1,31); k1 *= c2; h1 ^= k1;

        h1 = rotl64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

        k2 *= c2; k2  = rotl64(k2,33); k2 *= c1; h2 ^= k2;

        h2 = rotl64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
    }

    //----------
    // tail

    // Advance offset to the unprocessed tail of the data.
    offset += nblocks * 16;

    long k1 = 0;
    long k2 = 0;

    switch(length & 15)
    {
        case 15: k2 ^= ((long) key[offset+14]) << 48;
        case 14: k2 ^= ((long) key[offset+13]) << 40;
        case 13: k2 ^= ((long) key[offset+12]) << 32;
        case 12: k2 ^= ((long) key[offset+11]) << 24;
        case 11: k2 ^= ((long) key[offset+10]) << 16;
        case 10: k2 ^= ((long) key[offset+9]) << 8;
        case  9: k2 ^= ((long) key[offset+8]) << 0;
            k2 *= c2; k2  = rotl64(k2,33); k2 *= c1; h2 ^= k2;

        case  8: k1 ^= ((long) key[offset+7]) << 56;
        case  7: k1 ^= ((long) key[offset+6]) << 48;
        case  6: k1 ^= ((long) key[offset+5]) << 40;
        case  5: k1 ^= ((long) key[offset+4]) << 32;
        case  4: k1 ^= ((long) key[offset+3]) << 24;
        case  3: k1 ^= ((long) key[offset+2]) << 16;
        case  2: k1 ^= ((long) key[offset+1]) << 8;
        case  1: k1 ^= ((long) key[offset]);
            k1 *= c1; k1  = rotl64(k1,31); k1 *= c2; h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= length; h2 ^= length;

    h1 += h2;
    h2 += h1;

    h1 = fmix(h1);
    h2 = fmix(h2);

    h1 += h2;
    h2 += h1;

    result[0] = h1;
    result[1] = h2;
}
