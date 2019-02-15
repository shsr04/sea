#include "dyckmatchingstructure.h"
#include <iostream>

namespace Sealib {

uint64_t Sealib::DyckMatchingStructure::getMatchNaive(
    const BlockBitset &word,
    uint64_t idx) {
    uint32_t j = 0;
    uint32_t p = 0;
    std::vector<uint32_t> stack(word.size());
    do {
        if (word[j]) {  // '('
            stack[p++] = j;
        } else {
            if (p == 0) {
                return idx;
            }
            uint32_t i = stack[--p];
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

const BlockBitset &Sealib::DyckMatchingStructure::getWord() const {
    return word;
}

Sealib::DyckMatchingStructure::DyckMatchingStructure(const BlockBitset &word_) :
    word(word_) {}
Sealib::DyckMatchingStructure::DyckMatchingStructure(BlockBitset &&word_) :
    word(word_) {}

Sealib::DyckMatchingStructure::~DyckMatchingStructure() = default;

uint64_t Sealib::DyckMatchingStructure::getMatch(uint64_t idx) {
    return getMatchNaive(word, idx);
}

}  // namespace Sealib
