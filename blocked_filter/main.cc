#include <memory>
#include "BloomFilter.h"
#include "BloomFilterFactory.h"
#include <stdio.h>
#include <string.h>
#include <set>
#include <random>
#include <chrono>
using namespace std::chrono;

char* generateRandomString(int length) {
    char* randomString = new char[length + 1];

    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]-=./!@#$%^&*()_+{}|:<>?";
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
    auto filter = BloomFilterFactory::createFilter_benchmark();
    for (; set.size() < 1000000; ) {
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
    
	int trial = 100000;
	int i = 0; int fp = 0;

    int64_t t = 0;

    while (i < trial) {
    	char* str = generateRandomString(10);
        string s(str);
        if (set.find(s) != set.end()) {delete[] str; continue;}
        auto start = high_resolution_clock::now();
        bool isThere = filter->isPresent(str, strlen(str));
        auto end = high_resolution_clock::now();
        if (isThere) {
            fp++;
        }
        auto duration = duration_cast<microseconds>(end - start);
        t += duration.count();
        i++;
        delete [] str;
    }
    printf("fp rate: %f\n", ((double)fp) / trial);
    printf("Time: %f\n second", (double)(t) / trial);
}
