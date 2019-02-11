#include <sealib/dictionary/sharedrankselect.h>
#include "localselecttable.h"
#include <iostream>
#include <utility>

Sealib::SharedRankSelect::SharedRankSelect(
    std::shared_ptr<const Sealib::Bitset<uint8_t>> bitset_) :
    rankStructure(std::move(bitset_)),
    firstInSegment(generateFirstInBlockBitSet(rankStructure)) {
}

uint Sealib::SharedRankSelect::select(uint k) const {
    if (k == 0 || rankStructure.getSegmentCount() == 0) {
        return (uint) -1;
    }
    uint firstInSegmentRank = firstInSegment.rank(k);
    if (firstInSegmentRank == (uint) -1) {
        return (uint) -1;
    }
    uint h = rankStructure.getNonEmptySegments()[firstInSegmentRank - 1];
    uint8_t segment = rankStructure.getBitset().getBlock(h);
    auto localIndex = static_cast<uint8_t>(k - rankStructure.setBefore(h) - 1);
    uint8_t localSelect = LocalSelectTable::getLocalSelect(segment, localIndex);
    return localSelect + rankStructure.getSegmentLength() * h + 1;
}

Sealib::SharedRankSelect::SharedRankSelect() = default;

std::shared_ptr<const Sealib::Bitset<uint8_t>>
Sealib::SharedRankSelect::generateFirstInBlockBitSet(const SharedRankStructure &rs) {
    uint size = rs.rank(rs.size());
    if (size == (uint) -1) {
        size = 0;
    }
    std::shared_ptr<Bitset<uint8_t>> firstInBlockBitSet(new Bitset<uint8_t>(size));

    for (uint i = 0; i < rs.getSegmentCount(); i++) {
        uint8_t segment = rs.getBitset().getBlock(i);
        uint8_t localFirst = LocalSelectTable::getLocalSelect(segment, 0);
        if (localFirst != (uint8_t) -1) {  // has a local first, i.e. is not an empty segment
            // setBefore gives us the index in firstInBlockBitset
            uint before = rs.setBefore(i);
            (*firstInBlockBitSet)[before] = 1;
        }
    }

    return firstInBlockBitSet;
}

Sealib::SharedRankSelect::~SharedRankSelect() = default;

uint Sealib::SharedRankSelect::rank(uint k) const {
    return rankStructure.rank(k);
}
