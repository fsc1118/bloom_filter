#include <memory>
#include "BitSet.h"
#include "BloomFilter.h"
#include "BloomFilterFactory.h"
#include <stdio.h>

#include <string.h>
#include <set>
#include <random>


char* generateRandomString(int length) {
    char* randomString = new char[length + 1];
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distribution(0, sizeof(charset) - 1);

    for (int i = 0; i < length; i++) {
        randomString[i] = charset[distribution(generator)];
    }
    randomString[length] = '\0'; // null terminate the string

    return randomString;
}

int main()
{
    
    std::set<std::string> set;
    auto filter = BloomFilterFactory::getFilter(1000000, 0.01);
    for (int i = 0; i < 1000; i++) {
        // Random string
        char* str = generateRandomString(i);
        set.insert(str);
        filter->add(str, strlen(str));
    }
    
    // Iterate through the set and check if the filter returns true
    for (auto it = set.begin(); it != set.end(); it++) {
        if (!filter->isPresent(it->c_str(), it->length())) {
            printf("Error: %s not found in filter\n", it->c_str());
        }
    }
}