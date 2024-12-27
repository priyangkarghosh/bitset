#include <assert.h>
#include "bitset.h"
#include "malloc.h"
#include <string.h>

const int GROUP_LENGTH_BYTES = sizeof(unsigned long long);
const int GROUP_LENGTH_BITS = GROUP_LENGTH_BYTES * 8;
const unsigned long long FULL_GROUP_VALUE = ULLONG_MAX;

Bitset* bs_create(int length) {
    // scale by 8 since there are 8 bits in a byte
    int numGroups = length / GROUP_LENGTH_BITS + 1;
    Bitset* bitset = malloc(sizeof(int) * 2 + GROUP_LENGTH_BYTES * numGroups);
    assert(bitset); // if this fails then memory was not allocated properly

    // initialize the groups to 0, and the properties of the bitset
    memset(bitset->groups, 0, GROUP_LENGTH_BYTES * numGroups);
    bitset->length = length; bitset->numGroups = numGroups;
    return bitset;
}

void bs_flip(Bitset* bitset, int index) {
    // make sure the index entered is lower than the maximum entries the bitset can hold
    assert(index < bitset->length);

    // find the id of the group we are trying to access
    int groupId = index / GROUP_LENGTH_BITS;

    // find the index of which bit should be flipped in terms of the right hand side
    int bitshift = bs_flipIndexing(index % GROUP_LENGTH_BITS);

    // create a mask with a 1 at index of bitshift
    // -> i.e. if bitshift is 4, and the number is made of 8 bits
    //    the mask would be 00010000
    bitset->groups[groupId] = bitset->groups[groupId] ^ (1ULL << bitshift);
}

int bs_empty(Bitset* bitset) {
    // declare var to hold each group
    unsigned long long group;

    // loop through all groups to look for an empty one
    for (int i = 0; i < bitset->numGroups; i++) {
        // find the first group with a free index
        group = bitset->groups[i];

        // if the group is empty return the first element of the group
        if (!group) return i * bitset->numGroups;

        // check if the group is full
        if (group == FULL_GROUP_VALUE) continue;

        // otherwise find the first 0 in the group, starting from the left hand side
        // -> log2(x) gives you a number between 0 - # bits, which specifies the left most bit which is a 1
        // -> i.e. 2 can be represented as 10 in binary, log2(2) = 1
        //         counting from right to left, we can see that the index 1 is a 1
        //         assuming we start counting at 0
        // since we are looking for the first 0 in the group, the complemented number returns the first 0
        return i * bitset->numGroups + bs_flipIndexing(log2_ulonglong(~group));
    }
    return -1;
}

int log2_ulonglong (unsigned long long value) {
    const int tab64[64] = {
            63,  0, 58,  1, 59, 47, 53,  2,
            60, 39, 48, 27, 54, 33, 42,  3,
            61, 51, 37, 40, 49, 18, 28, 20,
            55, 30, 34, 11, 43, 14, 22,  4,
            62, 57, 46, 52, 38, 26, 32, 41,
            50, 36, 17, 19, 29, 10, 13, 21,
            56, 45, 25, 31, 35, 16,  9, 12,
            44, 24, 15,  8, 23,  7,  6,  5
    };

    value |= value >> 1; value |= value >> 2; value |= value >> 4;
    value |= value >> 8; value |= value >> 16; value |= value >> 32;
    return tab64[((unsigned long long)((value - (value >> 1))*0x07EDD5E59A4E28C2)) >> 58];
}

int bs_flipIndexing(int n) {
    assert(n < GROUP_LENGTH_BITS);
    return GROUP_LENGTH_BITS - n - 1;
}