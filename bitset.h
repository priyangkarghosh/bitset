#ifndef BITSET_H
#define BITSET_H

extern const int GROUP_LENGTH_BYTES;
extern const int GROUP_LENGTH_BITS;
extern const unsigned long long FULL_GROUP_VALUE;

typedef struct {
    int length; int numGroups;
    unsigned long long groups[];
} Bitset;

Bitset* bs_create(int length);
void bs_flip(Bitset* bitset, int index); // flips an element of the bitset
int bs_empty(Bitset* bitset); // returns index of first 0 element in the bitset, if there are none, returns -1
int bs_flipIndexing(int n); // flips between right to left indexing, and left to right indexing (useful since binary is read right to left)

//https://stackoverflow.com/questions/11376288/fast-computing-of-log2-for-64-bit-integers
int log2_ulonglong (unsigned long long value);
#endif
