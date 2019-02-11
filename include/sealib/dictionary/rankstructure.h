#ifndef SEALIB_DICTIONARY_RANKSTRUCTURE_H_
#define SEALIB_DICTIONARY_RANKSTRUCTURE_H_
#define CHECK_BIT(var, pos) (((var)>>(pos)) & 1)

#include <sealib/collection/bitset.h>
#include <memory>
#include <vector>

/**
 * Space efficient RankStructure implementation.
 * @author Johannes Meintrup
 */
namespace Sealib {
class RankStructure {
 protected:
    static constexpr const uint8_t segmentLength = 8;
    const Sealib::Bitset<uint8_t> bitset;
    uint segmentCount;
    uint maxRank;

    std::vector<uint> setCountTable;
    std::vector<uint> nonEmptySegments;

 public:
    uint64_t size() const;
    uint getMaxRank() const;
    const std::vector<uint> &getSetCountTable() const;
    const std::vector<uint> &getNonEmptySegments() const;

    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    uint64_t rank(uint64_t k) const;

    /**
     * @param bitset used for Rank
     */
    explicit RankStructure(const Sealib::Bitset<uint8_t> &bitset);

    /**
     * default empty constructor
     */
    RankStructure();

    /**
     * @return segment length
     */
    uint8_t getSegmentLength() const;

    /**
     * @return segment count
     */
    uint getSegmentCount() const;

    /**
     * @return segment of the bitset
     */
    const Sealib::Bitset<uint8_t>& getBitset() const;

    ~RankStructure();
    uint setBefore(uint64_t segment) const;
};
}  // namespace Sealib
#endif  // SEALIB_DICTIONARY_RANKSTRUCTURE_H_
