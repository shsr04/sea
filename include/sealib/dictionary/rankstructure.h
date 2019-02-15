#ifndef SEALIB_DICTIONARY_RANKSTRUCTURE_H_
#define SEALIB_DICTIONARY_RANKSTRUCTURE_H_
#define CHECK_BIT(var, pos) (((var)>>(pos)) & 1)

#include <sealib/collection/blockbitset.h>
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
    BlockBitset const &bitset;
    uint32_t segmentCount;
    uint32_t maxRank;

    std::vector<uint32_t> setCountTable;
    std::vector<uint32_t> nonEmptySegments;

 public:
    uint64_t size() const;
    uint32_t getMaxRank() const;
    const std::vector<uint32_t> &getSetCountTable() const;
    const std::vector<uint32_t> &getNonEmptySegments() const;

    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    uint64_t rank(uint64_t k) const;

    explicit RankStructure(BlockBitset const &);
    explicit RankStructure(BlockBitset &&);

    /**
     * @return segment length
     */
    uint8_t getSegmentLength() const;

    /**
     * @return segment count
     */
    uint32_t getSegmentCount() const;

    /**
     * @return segment of the bitset
     */
    const BlockBitset& getBitset() const;

    ~RankStructure();
    uint32_t setBefore(uint64_t segment) const;
};
}  // namespace Sealib
#endif  // SEALIB_DICTIONARY_RANKSTRUCTURE_H_
