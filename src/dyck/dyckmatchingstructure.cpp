#include "dyckmatchingstructure.h"
#include <iostream>

uint Sealib::DyckMatchingStructure::getMatchNaive(
    const Sealib::Bitset<uint8_t> &word,
    uint idx) {
    uint j = 0;
    uint p = 0;
    std::vector<uint> stack(word.size());
    do {
        if (word[j]) {  // '('
            stack[p++] = j;
        } else {
            if (p == 0) {
                return idx;
            }
            uint i = stack[--p];
            if (idx == i) {
                return j;
            }
            if (idx == j) {
                return i;
            }
        }
        j++;
    } while (j != word.size());

    return idx;;
}

const Sealib::Bitset<uint8_t> &Sealib::DyckMatchingStructure::getWord() const {
    return word;
}

Sealib::DyckMatchingStructure::DyckMatchingStructure(const Sealib::Bitset<uint8_t> &word_) :
    word(word_) {}

Sealib::DyckMatchingStructure::~DyckMatchingStructure() {}

uint Sealib::DyckMatchingStructure::getMatch(uint idx) {
    return getMatchNaive(word, idx);
}
