#ifndef SEALIB_COLLECTION_BITSET_H_
#define SEALIB_COLLECTION_BITSET_H_

#include <cstdint>
#include <vector>
#include <cassert>
#include <limits>
#include <memory>
#include "sealib/collection/sequence.h"

namespace Sealib {

template <typename BlockType = uint,
    typename AllocatorType = std::allocator<BlockType> >
class Bitset;
}

namespace Sealib {
/**
 * Simple Bitset to be used by all classes that need a dynamically allocated bitset.
 * Adds the ability to simply address whole blocks in the bitset for table lookup convenience.
 * Includes an implementation of operator[] for convenient use.
 * Partly based on dynamic_bitset from the boost library.
 * @author Johannes Meintrup
 */
template<typename BlockType, typename AllocatorType>
class Bitset final : Sequence<bool, uint> {
    static const uint bitsPerBlock = sizeof(BlockType) * 8;
    static const BlockType BlockTypeOne = BlockType(1);

    uint bits;
    std::vector<BlockType, AllocatorType> mbits;

    inline bool get(const BlockType &i, uint b) const {
        return static_cast<bool>(i & (BlockTypeOne << b));
    }

 public:
    static const uint npos = std::numeric_limits<uint>::max();

    explicit Bitset(uint bits_);

    explicit Bitset(const std::vector<bool> &bitvector);

    Bitset();

    class BitReference;

    /**
     * @param bit idx of the bit
     * @return true if the bit is set, false otherwise
     */
    bool get(uint bit) const override;

    /**
     * @param bit index to insert to
     * @param value 0/1 to unset/set the bit
     */
    void insert(uint bit, bool value) override;

    /**
      * non-const version of the [] operator, uses the BitReference wrapper class.
      * @param bit index of the bit
      * @return BitReference referencing the block and index of the bit.
      */
     BitReference operator[](uint bit);

     /**
      * const version of the operator needs only a simple get instead of the BitReference wrapper class.
      * @param bit index of the bit
      * @return true if set, false otherwise
      */
     bool operator[](uint bit) const;

    /**
     * sets all bits to true
     */
    void set();

    /**
     * clears all bits
     */
    void clear();

    /**
     * flip bitset
     */
    void flip();

    /**
     * @return number of bits held by bitset
     */
    uint size() const { return bits; }

    /**
     * @return number of blocks used to store bits
     */
    uint blocks() const { return (uint)mbits.size(); }

    /**
     * Proxy class to simulate lvalues of bit type.
     * Implementation taken from boost dynamic_bitset.
     */
     class BitReference {
         friend class Bitset<BlockType, AllocatorType>;

         BitReference(BlockType *b, BlockType pos) :
                 mblock(b),
                 mmask(static_cast<BlockType>(BlockType(1) << pos))
         {}

         BitReference(BitReference const &) = default;

      private:
         BlockType *mblock;
         const BlockType mmask;

         inline void doSet() { *mblock = *mblock | mmask; }
         inline void doReset() { *mblock = *mblock & static_cast<BlockType>(~mmask); }
         inline void doFlip() { *mblock = *mblock ^ mmask; }
         inline void doAssign(bool b) { b ? doSet() : doReset(); }

      public:
         operator bool() const { return (*mblock & mmask) != 0; }
         bool operator~() const { return (*mblock & mmask) == 0; }

         BitReference& flip() {
             doFlip();
             return *this;
         }

         // for b[i] = x
         BitReference& operator=(bool x) {
             doAssign(x);
             return *this;
         }

         // for b[i] = b[j]
         BitReference& operator=(const BitReference& rhs) {
             doAssign(rhs);
             return *this;
         }

         BitReference& operator|=(bool x) {
             if (x) doSet();
             return *this;
         }

         BitReference& operator&=(bool x) {
             if (!x) doReset();
             return *this;
         }

         BitReference& operator^=(bool x) {
             if (x) doFlip();
             return *this;
         }

         BitReference& operator-=(bool x) {
             if (x) doReset();
             return *this;
         }
     };

    /**
     * @param idx of the block
     * @return const ref to the block
     */
    const BlockType& getBlock(uint idx) const;

    /**
     * @param idx of the bit the block should start at, it will create a new block starting at idx until idx+blocksize.
     * This way you can get a block that is somewhere in the bitset, but residing in two actual blocks.
     * @return the created new block
     */
    BlockType getShiftedBlock(uint idx) const;

    /**
     * @param idx of the block
     * @param block value to be set
     */
    void setBlock(uint idx,  BlockType block);

    /**
     * @return allocator used for allocation of the internal storage
     */
    AllocatorType get_allocator() const {
        return mbits.get_allocator();
    }

    //  basic bitset operations
    Bitset<BlockType, AllocatorType>& operator&=(const Bitset<BlockType, AllocatorType>& rhs);

    Bitset<BlockType, AllocatorType>& operator|=(const Bitset<BlockType, AllocatorType>& rhs);

    Bitset<BlockType, AllocatorType>& operator^=(const Bitset<BlockType, AllocatorType>& rhs);

    Bitset<BlockType, AllocatorType>& operator-=(const Bitset<BlockType, AllocatorType>& rhs);

    Bitset<BlockType, AllocatorType> operator~() const;

    /**
     * resizes the bitset to hold up to size bits
     * @param size - size of the bitset after resizing
     */
    void resize(uint size);
};

template <typename Block, typename Allocator>
Bitset<Block, Allocator>
operator&(const Bitset<Block, Allocator>& lhs, const Bitset<Block, Allocator>& rhs) {
    Bitset<Block, Allocator> b(lhs);
    return b &= rhs;
}

template <typename Block, typename Allocator>
Bitset<Block, Allocator>
operator|(const Bitset<Block, Allocator>& lhs, const Bitset<Block, Allocator>& rhs) {
    Bitset<Block, Allocator> b(lhs);
    return b |= rhs;
}

template <typename Block, typename Allocator>
Bitset<Block, Allocator>
operator^(const Bitset<Block, Allocator>& lhs, const Bitset<Block, Allocator>& rhs) {
    Bitset<Block, Allocator> b(lhs);
    return b ^= rhs;
}

template <typename Block, typename Allocator>
Bitset<Block, Allocator>
operator-(const Bitset<Block, Allocator>& lhs, const Bitset<Block, Allocator>& rhs) {
    Bitset<Block, Allocator> b(lhs);
    return b -= rhs;
}

}  // namespace Sealib
#endif  // SEALIB_COLLECTION_BITSET_H_
