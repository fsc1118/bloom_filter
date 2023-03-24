#include "BloomFilter.h"
#include "assert.h"
int main()
{
    BloomFilter bf(1000000);
    bf.add("hello", 5);
    bf.add("world", 5);
    assert(bf.isPresent("hello", 5));
    assert(bf.isPresent("world", 5));
    assert(!bf.isPresent("foo", 3));
    assert(bf.isPresent("bar", 3));
    return 0;
}