#include <set>
#include <memory>
#include <string>
#include <random>
#include <string.h>
#include "BitSet.h"
#include "BloomFilter.h"

#include <chrono>
using namespace std;
using namespace std::chrono;
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
    BloomFilter bf(6, shared_ptr<BitSet>(new BitSet((1 << 17) << 9)));
    set<string> set;
    for (int i = 0; i < 1000000; i++) {
        char* str = generateRandomString(10);
        std::string s(str);
        if (set.find(s) != set.end()) {
            delete[] str;
            continue;
        }
        bf.add(str, strlen(str));
        set.insert(s);
    }
    int trial = 100000;
	int i = 0; int fp = 0;

    int64_t t = 0;

    while (i < trial) {
    	char* str = generateRandomString(10);
        string s(str);
        if (set.find(s) != set.end()) {delete[] str; continue;}
        auto start = high_resolution_clock::now();
        bool isThere = bf.isPresent(str, strlen(str));
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
    return 0;
}
