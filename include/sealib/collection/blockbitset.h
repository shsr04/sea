#ifndef SEALIB_COLLECTION_BLOCKBITSET_H_
#define SEALIB_COLLECTION_BLOCKBITSET_H_
#include <vector>
#include "sealib/_types.h"

namespace Sealib {
/**
 * Experimental bitset with block access.
 * It should be safe to access `bit` like a normal bit vector.
 * However, the other members support only a few safe operations:
 * - You can use the [] operator on any member.
 * - You can access the underlying array with data().
 * - You can use std::swap to swap contents of a member and an outside vector.
 *
 *  Example:
 *      BlockBitset b(20);
 *      b.bit[5]=1;
 *      b.byte[1]=20*b.bit[5];
 *      ...
 *
 * @author Simon Heuser
 */
union BlockBitset {
    /**
     * Creates a bitset of the given size.
     * @param size Number of bits in the bitset
     */
    BlockBitset(uint64_t size) : bit(size) {}
    BlockBitset(std::vector<bool> const &v) : bit(v) {}
    BlockBitset(BlockBitset const &p) : bit(p.bit) {}
    ~BlockBitset() { bit.~vector(); }

    std::vector<bool>::reference operator[](uint64_t i) {
        return bit[i];
    }
    bool operator[](uint64_t i) const { return bit[i]; }

    size_t size() const { return bit.size(); }

    uint8_t getShiftedBlock(uint64_t i) const {
        uint8_t len = 32;
        uint8_t b1 = bit[i/len];
        uint8_t b2 = bit[(i+len-1)/len];
        uint8_t bitIdx = static_cast<uint8_t>(i % len);
        return static_cast<uint8_t>((b1 >> bitIdx) | (b2 << (len - bitIdx)));
    }

    /**
     * Member that allows reading and writing specific bits.
     */
    std::vector<bool> bit;
    /**
     * Member that allows reading and writing bytes.
     */
    std::vector<uint8_t> byte;
    /**
     * Member that allows reading and writing 64-bit words.
     */
    std::vector<uint64_t> word;
};
}  // namespace Sealib
#endif  // SEALIB_COLLECTION_BLOCKBITSET_H_
