#ifndef SRC_DYCK_DYCKMATCHINGSTRUCTURE_H_
#define SRC_DYCK_DYCKMATCHINGSTRUCTURE_H_

#include <sealib/collection/blockbitset.h>
#include <iostream>

namespace Sealib {
/**
 * Base class for dyckmatching.
 * Enables to find the match of a parenthesis in a dyck word w in a naive way, taking O(n) time.
 *
 * Also the static functions to find a match in a dyck word w in a naive way.
 */
class DyckMatchingStructure {
 public:
    /**
     * Finds the match of a parenthesis in a dyck word in O(n) using O(n) space at worst.
     * @param word_ - valid dyck word
     * @param idx - idx of the parenthesis in word_ for which the match should be found
     * @return match of word_(idx_), or idx_ if there is no match. (if word_ is not valid)
     */
    static uint64_t getMatchNaive(
        const BlockBitset &word_,
        uint64_t idx);

    /**
     * Finds the match of a parenthesis in a dyck word.
     * @param idx - idx of the parenthesis in word_ for which the match should be found
     * @return match of word_(idx_), or idx_ if there is no match. (if word_ is not valid)
     */
    virtual uint64_t getMatch(uint64_t idx);

    /**
     * constructs a matching structure for the given dyck word word_.
     * There is no test to check if word_ is a valid dyck word.
     * @param word_ - dyck word for the matching structure
     */
    explicit DyckMatchingStructure(const BlockBitset &word_);
    explicit DyckMatchingStructure(BlockBitset &&word_);

    /**
     * Default descructor.
     */
    virtual ~DyckMatchingStructure();

    /**
     * @return BlockBitset word
     */
    const BlockBitset &getWord() const;


 protected:
    const BlockBitset &word;
    static constexpr const uint8_t mSegmentLength = 7;
};
}  // namespace Sealib
#endif  // SRC_DYCK_DYCKMATCHINGSTRUCTURE_H_
