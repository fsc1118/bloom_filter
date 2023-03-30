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

    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distribution(0, strlen(charset) - 1);
    for (int i = 0; i < length; i++) {
        randomString[i] = charset[distribution(generator)];
    }
    randomString[length] = '\0'; // null terminate the string
    return randomString;
}

int main()
{ 
    std::set<std::string> set;
    auto filter = BloomFilterFactory::getFilter(10000, 0.001);
    for (; set.size()< 10000; ) {
        // Random string
        char* str = generateRandomString(10);
        std::string s(str);
        if (set.find(s) != set.end()) {
            delete[] str;
            continue;
        }
        set.insert(s);
        filter->add(str, strlen(str));
    }
    
    // Iterate through the set and check if the filter returns true
    for (auto it = set.begin(); it != set.end(); it++) {
        if (!filter->isPresent(it->c_str(), it->length())) {
            printf("Error: %s not found in filter\n", it->c_str());
        }
    }
	int trial = 1000000;
	int i = 0; int fp = 0;
    while (i < trial) {
    	char* str = generateRandomString(10);
        string s(str);
        if (set.find(s) != set.end()) {delete str; continue;}
        if (filter->isPresent(str, strlen(str))) {
            fp++;
        }
        i++;
    }
    printf("%f\n", ((double)fp) / trial);
}
