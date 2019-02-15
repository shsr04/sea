#include "localranktable.h"
#include <sealib/dictionary/rankstructure.h>
#include <iostream>
#include <cmath>
#include <utility>

namespace Sealib {

uint64_t Sealib::RankStructure::rank(uint64_t k) const {
    if (k == 0 || k > maxRank) {
        return (uint64_t) -1;
    }
    uint64_t segmentIdx = (k - 1) / segmentLength;
    uint8_t segment = bitset.byte[segmentIdx];
    auto localIdx = static_cast<uint8_t>((k - 1) % segmentLength);
    return setBefore(segmentIdx) + LocalRankTable::getLocalRank(segment, localIdx);
}

Sealib::RankStructure::RankStructure(const BlockBitset &bitset_) :
    bitset(bitset_),
    segmentCount(static_cast<uint32_t>(bitset.size() / segmentLength)) {
    auto lastSeg = static_cast<uint8_t>((bitset.size() % segmentLength));

    if ((lastSeg != 0) && bitset.bit.size() != 0) {
        segmentCount++;
    }

    maxRank =
        segmentCount == 0 ?
        0 : segmentLength * (lastSeg == 0 ? segmentCount : segmentCount - 1) + lastSeg;
    nonEmptySegments.reserve(segmentCount);

    for (uint32_t i = 0; i < segmentCount; i++) {
        uint8_t segment = bitset.byte[i];
        if (LocalRankTable::getLocalRank(segment, 7) != 0) {
            nonEmptySegments.push_back(i);
        }
    }

    if (segmentCount != 0) {
        setCountTable.reserve(segmentCount);
        uint32_t cnt = 0;
        for (uint64_t i = 0; i < segmentCount - 1; i++) {
            uint8_t segment = bitset.byte[i];
            cnt += LocalRankTable::getLocalRank(segment, 7);
            setCountTable.push_back(cnt);
        }
    }
}

uint32_t Sealib::RankStructure::getSegmentCount() const {
    return segmentCount;
}

uint32_t Sealib::RankStructure::setBefore(uint64_t segment) const {
    if (segment == 0) return 0;
    return setCountTable[segment - 1];
}

uint8_t Sealib::RankStructure::getSegmentLength() const {
    return segmentLength;
}
const BlockBitset &Sealib::RankStructure::getBitset() const {
    return bitset;
}
uint32_t Sealib::RankStructure::getMaxRank() const {
    return maxRank;
}
const std::vector<uint32_t> &Sealib::RankStructure::getSetCountTable() const {
    return setCountTable;
}
const std::vector<uint32_t> &Sealib::RankStructure::getNonEmptySegments() const {
    return nonEmptySegments;
}
uint64_t Sealib::RankStructure::size() const {
    return bitset.size();
}

}  // namespace Sealib
