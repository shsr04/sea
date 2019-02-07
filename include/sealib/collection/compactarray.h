#ifndef SEALIB_COLLECTION_COMPACTARRAY_H_
#define SEALIB_COLLECTION_COMPACTARRAY_H_

#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/bitset.h"
#include "sealib/collection/sequence.h"

using Sealib::Bitset;

namespace Sealib {
/**
 * Groups of bits, packed into an array of uints.
 * To get a bitset, create with v=2. To get a color vector, create with v=3 or
 * v=4.
 * For optimal space usage, see constructor comments.
 * @author Simon Heuser
 */
class CompactArray : public Sequence<uint> {
 public:
    /**
     * Create a new compact array.
     * @param size number of values the compact array should hold
     * @param v number of states for one value (e.g. v=2 for a binary array, v=4
     * for states 0,1,2,3). Make sure that ld(v) does not exceed bitsize(uint)!
     */
    explicit CompactArray(uint count, uint v = 3);

    /**
     * Insert a value to the given index.
     * @param i the destination index
     * @param p the value to insert (in [0,v])
     */
    void insert(uint i, uint p) override;

    /**
     * Get a value from the compact array.
     * @param i index to get the value from
     * @return the found value
     */
    uint get(uint i) const override;

    uint64_t byteSize() const { return data.capacity() * sizeof(uint); }

 private:
    uint valueWidth, singleMask;
    std::vector<uint> data;
};
}  // namespace Sealib
#endif  // SEALIB_COLLECTION_COMPACTARRAY_H_
