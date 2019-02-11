#ifndef SEALIB_DICTIONARY_CHOICEDICTIONARY_H_
#define SEALIB_DICTIONARY_CHOICEDICTIONARY_H_
#include <cstdint>
#include <vector>
#include "sealib/_types.h"

namespace Sealib {
/**
 * A choice dictionary is a bitset containing n elements that supports reading
 * and setting a bit in constant time and additionally a so-called choice
 * operation that returns the position of an arbitrary bit that is set to 1
 * in constant time.
 * @author Dennis Appel
 */
class ChoiceDictionary {
 private:
    static const uint SHIFT_OFFSET = 1,
        POINTER_OFFSET = 1,
        TUPEL_OFFSET = 1,
        TUPEL_FACTOR = 2;
        
    /**
     * Word size in bits
     */
    uint wordSize;
    /**
     * Stores the number of words in primary.
     */
    uint wordCount;
    /**
     * Points to the next available word in validator and
     * pointer-1 to the last linked word in validator.
     */
    uint pointer;
    /**
     * Array Structure where each word represents a subset of the entire bitset.
     */
    std::vector<uint> primary;
    /**
     * Tupel structure where the first word in a block contains a bitset 
     * where each bit that is set to 1 points to a subset in primary 
     * with at least one bit set to 1.
     * The second word is used for pointer validation with the validator array.
     */
    std::vector<uint> secondary;
    /**
     * Array used to validate entries in secondary with
     * validator[i] pointing to a tupel in secondary that has at least
     * one bit set to 1.
     */
    std::vector<uint> validator;

    void createDataStructure(uint size);

    void createPrimary();

    void createSecondary(uint secondarySize);

    void createValidator(uint validatorSize);

    /**
     * Updates secondary to represent updates in primary.
     * @param primaryIndex Index of the updated Word in primary.
     */
    void updateSecondary(uint primaryIndex);

    void removeFromSecondary(uint primaryIndex);

    /**
     * Creates a link between a tupel in secondary and validator.
     *
     * @param secondaryIndex Index of the new secondary word.
     */
    uint makeLink(uint secondaryIndex);

    void breakLink(uint secondaryIndex);

    void shrinkValidator(uint startIndex);

    bool isInitialized(uint primaryIndex);

    bool hasColor(uint primaryIndex);

 public:
    /**
     * Creates choice dictionary with given size
     * @param length Length of the choice dictionary
     */
    explicit ChoiceDictionary(uint size);

    /**
     * Sets a bit at specified index to 1.
     * @param index Index of bit that should be set to 1.
     */
    void insert(uint index);

    /**
     * Returns the bit at specified index.
     * @param index Index to read.
     */
    bool get(uint index);

    /**
     * Returns an arbitrary bit position that is set to 1.
     * @throws std::exception if empty
     */
    uint choice();

    /**
     * Sets a bit at specified index to 0.
     * @param index Index of bit that should be set to 0.
     * @throws std::exception if empty
     */
    void remove(uint index);

    uint getPrimaryWord(uint primaryIndex);

    uint getSecondaryWord(uint secondaryIndex);

    uint getPointerTarget(uint nextPointer);

    bool pointerIsValid(uint nextPointer);

    uint getWordSize();

    uint getSecondarySize();
};
}  // namespace Sealib
#endif  // SEALIB_DICTIONARY_CHOICEDICTIONARY_H_
