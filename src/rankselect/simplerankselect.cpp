#include "sealib/dictionary/simplerankselect.h"

uint Sealib::SimpleRankSelect::select(uint k) const {
    if (k == 0 || k > selects.size()) return (uint) -1;
    return selects[k - 1];
}

uint Sealib::SimpleRankSelect::rank(uint k) const {
    if (k == 0 || k > ranks.size()) return (uint) -1;
    return ranks[k - 1];
}

Sealib::SimpleRankSelect::SimpleRankSelect(
    std::shared_ptr<const Sealib::Bitset<uint8_t>> bitset_) :
    bitset(std::move(bitset_)),
    ranks(bitset->size()),
    selects(bitset->size(), (uint) -1) {
    uint rank = 0;
    for (uint i = 0; i < bitset->size(); i++) {
        if ((*bitset)[i]) {
            selects[rank++] = i + 1;
        }
        ranks[i] = rank;
    }
    selects.resize(rank);
}
