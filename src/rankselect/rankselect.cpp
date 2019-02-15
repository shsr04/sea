#include <sealib/dictionary/rankselect.h>
#include "localselecttable.h"
#include <iostream>
#include <utility>

namespace Sealib {

Sealib::RankSelect::RankSelect(BlockBitset const &bitset_) 
    : rankStructure(bitset_),
    firstInSegment(generateFirstInBlockBitSet(rankStructure)) {}

uint64_t Sealib::RankSelect::select(uint64_t k) const {
    if (k == 0 || rankStructure.getSegmentCount() == 0) {
        return (uint64_t) -1;
    }
    uint64_t firstInSegmentRank = firstInSegment.rank(k);
    if (firstInSegmentRank == (uint64_t) -1) {
        return (uint64_t) -1;
    }
    uint64_t h = rankStructure.getNonEmptySegments()[firstInSegmentRank - 1];
    uint8_t segment = rankStructure.getBitset().byte[h];
    auto localIndex = static_cast<uint8_t>(k - rankStructure.setBefore(h) - 1);
    uint8_t localSelect = LocalSelectTable::getLocalSelect(segment, localIndex);
    return localSelect + rankStructure.getSegmentLength() * h + 1;
}

BlockBitset Sealib::RankSelect::generateFirstInBlockBitSet(const RankStructure &rs) {
    uint64_t size = rs.rank(rs.size());
    if (size == (uint64_t) -1) {
        size = 0;
    }
    BlockBitset firstInBlockBitSet(size);

    for (uint64_t i = 0; i < rs.getSegmentCount(); i++) {
        uint8_t segment = rs.getBitset().byte[i];
        uint8_t localFirst = LocalSelectTable::getLocalSelect(segment, 0);
        if (localFirst != (uint8_t) -1) {  // has a local first, i.e. is not an empty segment
            // setBefore gives us the index in firstInBlockBitset
            uint32_t before = rs.setBefore(i);
            firstInBlockBitSet[before] = 1;
        }
    }

    return firstInBlockBitSet;
}

uint64_t Sealib::RankSelect::rank(uint64_t k) const {
    return rankStructure.rank(k);
}
uint64_t Sealib::RankSelect::size() const {
    return rankStructure.size();
}

}  // namespace Sealib
