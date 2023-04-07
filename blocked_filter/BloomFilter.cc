#include "BloomFilter.h"
#include "Murmurhash.h"
#include <assert.h>
#include <malloc.h>
#include <memory>
#include <stdio.h> // debug
#include <string.h>
#define SECTOR_SIZE 8
#define k 6
#define LOG_BLOCK_SIZE 17

BloomFilter::BloomFilter(int hashCount, long numBits) {
  this->hashCount = hashCount;
  // Allign 64 byte (Cache line)
  this->bitSet = (char *)memalign(64, numBits >> 3);
  assert(this->bitSet != NULL);
}

void BloomFilter::add(const char *key, int length) {
  long *hash_store = new long[2];
  MurmurHash::hash(key, 0, length, 0, hash_store);
  uint64_t hash = hash_store[0];
  // load the block
  int blockIndex = hash & ((1 << LOG_BLOCK_SIZE) - 1);
  char *blockAddress = this->bitSet + (blockIndex << 6);
  hash = hash >> (LOG_BLOCK_SIZE);
  // 6 bit in total to set, each iteration set 2 bit
  for (int i = 0; i < (k >> 1); i++) {
    // Use last 3 bit of hash to find sector (64 bit)
    int sector = hash & 0x7;
    hash = hash >> 3;
    // Load the word
    char *word = blockAddress + (sector << 3);
    // Use 6 bit of hash to find a bit
    int bitIndex1 = hash & 63;
    hash = hash >> 6;
    int bitIndex2 = hash & 63;
    hash = hash >> 6;
    // set the bit at bitIndex1 and bitIndex2
    long bitSetted = (1 << bitIndex1) | (1 << bitIndex2);
    // set the bit
    long *wordPtr = (long *)word;
    *wordPtr = *wordPtr | bitSetted;
  }
}

bool BloomFilter::isPresent(const char *key, int length) {
  long *hash_store = new long[2];
  MurmurHash::hash(key, 0, length, 0, hash_store);
  uint64_t hash = hash_store[0];
  // load the block
  int blockIndex = hash & ((1 << LOG_BLOCK_SIZE) - 1);
  char *blockAddress = this->bitSet + (blockIndex << 6);
  // 6 bit to set, each iteration set 2 bit
  hash = hash >> (LOG_BLOCK_SIZE);
  for (int i = 0; i < (k >> 1); i++) {
    // Use last 3 bit of hash
    int sector = hash & 0x7;
    hash = hash >> 3;
    // Load the word
    char *word = blockAddress + (sector << 3);
    // Use last 6 bit of hash
    int bitIndex1 = hash & 63;
    hash = hash >> 6;
    int bitIndex2 = hash & 63;
    hash = hash >> 6;

    // set the bit at bitIndex1 and bitIndex2
    long bitSetted = (1 << bitIndex1) | (1 << bitIndex2);

    // check the bit
    long *wordPtr = (long *)word;
    if (((*wordPtr) & bitSetted) != bitSetted) {
      return false;
    }
  }
  return true;
}
