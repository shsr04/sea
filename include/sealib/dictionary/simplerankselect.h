#ifndef SEALIB_DICTIONARY_SIMPLERANKSELECT_H_
#define SEALIB_DICTIONARY_SIMPLERANKSELECT_H_

#include <sealib/collection/bitset.h>
#include <memory>
#include <vector>

namespace Sealib {
/**
 * Naive implementation of RankSelect, used to test the space efficient variant or for debugging
 * @author Johannes Meintrup
 */
class SimpleRankSelect {
 private:
    std::shared_ptr<const Bitset<uint8_t>> bitset;
    std::vector<uint> ranks;
    std::vector<uint> selects;

 public:
    /**
     * Selects the k-th set bit
     * @param k idx
     * @return k-th set bit
     */
    uint select(uint k) const;

    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    uint rank(uint k) const;

    explicit SimpleRankSelect(std::shared_ptr<const Bitset<uint8_t>> bitset);
};
}  // namespace Sealib
#endif  // SEALIB_DICTIONARY_SIMPLERANKSELECT_H_
