#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "BitSet.h"
BitSet::BitSet(long numBits) {
    if (numBits < 0) {
        fprintf(stderr, "numBits must be positive");
        exit(1);
    }
    /** returns the number of 64 bit words it would take to hold numBits */

    long wordCount = (((numBits - 1) >> 6) + 1); 
    if (wordCount > 0x7fffffff) {
        fprintf(stderr, "Bloom filter size is > 16GB, reduce the bloom_filter_fp_chance");
        exit(1);
    }
    long byteCount = wordCount * 8L;
    bytes = (char*)malloc(byteCount);
    if (bytes == NULL) {
        fprintf(stderr, "Out of native memory occured, You can avoid it by increasing the system ram space or by increasing bloom_filter_fp_chance.");
        exit(1);
    }
    size = byteCount;
    clear();
}

void BitSet::clear() {
    memset(bytes, 0, size);
}


// number of bits
long BitSet::capacity() {
    return size * 8L;
}

// get bit at index
bool BitSet::get(long index) {
    long i = index >> 3;
    long bit = index & 0x7;
    int bitmask = 0x1 << bit;
    return (bytes[i] & bitmask) != 0;
}

// set bit at index
void BitSet::set(long index) {
    long i = index >> 3;
    long bit = index & 0x7;
    char bitmask = 0x1 << bit;
    bytes[i] = (char)(bitmask | bytes[i]);
}