#include <memory>
#include "BitSet.h"
#include "BloomFilter.h"
#include "BloomFilterFactory.h"
#include <stdio.h>
int main()
{
    auto filter = BloomFilterFactory::getFilter(100, 0.01);
    filter->add("hello", 5);
    filter->add("world", 5);
    printf("%d\n", filter->isPresent("hello", 5));
    printf("%d\n", filter->isPresent("world", 5));
    printf("%d\n", filter->isPresent("foo", 3));
}