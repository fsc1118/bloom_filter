#ifndef MURMURHASH_H
#define MURMURHASH_H

class MurmurHash
{
    private:
        static long rotl64(long v, int n);
        static long getBlock(const char *key, int offset, int index);
        static long fmix(long k);
    public:
        static void hash(const char *key, int offset, int length, long seed, long result[]);
};
#endif