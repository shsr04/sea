#include "sealib/dictionary/choicedictionary.h"
#include <exception>
#include <iostream>

using Sealib::ChoiceDictionary;

static const uint ONE = 1;

class emptyChoiceDictionary : public std::exception {
    const char* what() const noexcept {
        return "Choice dictionary is empty. Operations \'choice()\'"
               " and \'remove()\' are not possible";
    }
};

ChoiceDictionary::ChoiceDictionary(uint size)
    :   wordSize(sizeof(uint) * 8),
        wordCount(size / wordSize + 1),
        pointer(0),
        primary(wordCount),
        secondary((wordCount/(uint)wordSize+1)*TUPEL_FACTOR),
        validator(wordCount/(uint)wordSize+1)
    {}

void ChoiceDictionary::insert(uint index) {
    uint primaryWord;
    uint targetBit;

    uint primaryIndex = index / wordSize;
    uint primaryInnerIndex = index % wordSize;

    if (isInitialized(primaryIndex))
        primaryWord = primary[primaryIndex];
    else
        primaryWord = 0;

    targetBit = ONE << (wordSize - SHIFT_OFFSET - primaryInnerIndex);
    primary[primaryIndex] = primaryWord | targetBit;

    updateSecondary(primaryIndex);
}

bool ChoiceDictionary::get(uint index) {
    uint primaryWord;
    uint targetBit;
    uint primaryInnerIndex;

    uint primaryIndex = index / (uint)wordSize;

    if (!isInitialized(primaryIndex))
        return 0;

    primaryInnerIndex = index % (uint)wordSize;

    primaryWord = primary[primaryIndex];
    targetBit = ONE << (wordSize - SHIFT_OFFSET - primaryInnerIndex);

    return (primaryWord & targetBit) != 0;
}

uint ChoiceDictionary::choice() {
    uint colorIndex;
    uint primaryWord;
    uint primaryIndex;
    uint secondaryWord;
    uint primaryInnerIndex;

    if (pointer == 0) throw emptyChoiceDictionary();

    uint secondaryIndex = validator[pointer - POINTER_OFFSET] - TUPEL_OFFSET;
    secondaryWord = secondary[secondaryIndex];

    primaryIndex = (secondaryIndex / TUPEL_FACTOR) * wordSize +
                   (uint)__builtin_clzl(secondaryWord);

    primaryWord = primary[primaryIndex];

    colorIndex = (uint)primaryIndex * (uint)wordSize;
    primaryInnerIndex = (uint)__builtin_clzl(primaryWord);

    colorIndex += primaryInnerIndex;
    return colorIndex;
}

void ChoiceDictionary::remove(uint index) {
    uint primaryWord;
    uint newPrimaryWord;
    uint targetBit;

    if (pointer == 0) throw emptyChoiceDictionary();

    uint primaryIndex = index / (uint)wordSize;
    uint primaryInnerIndex = index % (uint)wordSize;

    if (!isInitialized(primaryIndex)) return;

    primaryWord = primary[primaryIndex];
    targetBit = ONE << (wordSize - SHIFT_OFFSET - primaryInnerIndex);
    newPrimaryWord = primaryWord & ~targetBit;

    primary[primaryIndex] = newPrimaryWord;

    if (newPrimaryWord == 0) {
        removeFromSecondary(primaryIndex);
    }
}

uint ChoiceDictionary::getPrimaryWord(uint primaryIndex) {
    return primary[primaryIndex];
}

uint ChoiceDictionary::getSecondaryWord(uint secondaryIndex) {
    return secondary[secondaryIndex];
}

uint ChoiceDictionary::getPointerTarget(uint nextPointer) {
    return validator[nextPointer] - TUPEL_OFFSET;
}

bool ChoiceDictionary::pointerIsValid(uint nextPointer) {
    if (nextPointer >= pointer) return false;

    uint secondaryIndex = validator[nextPointer];
    if (secondaryIndex > (wordCount / wordSize + 1) * TUPEL_FACTOR) return false;

    if (nextPointer == secondary[secondaryIndex])
        return true;
    else
        return false;
}

uint ChoiceDictionary::getWordSize() { return wordSize; }

uint ChoiceDictionary::getSecondarySize() {
    return wordCount / (uint)wordSize + 1;
}

void ChoiceDictionary::updateSecondary(uint primaryIndex) {
    uint targetBit;
    uint secondaryWord;
    uint secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;
    uint linkTarget = secondary[secondaryIndex + TUPEL_OFFSET];

    if (linkTarget <= wordCount / (uint)wordSize &&
        validator[linkTarget] == secondaryIndex + TUPEL_OFFSET && pointer > 0) {
        secondaryWord = secondary[secondaryIndex];
    } else {
        secondaryWord = 0;
        secondary[secondaryIndex + TUPEL_OFFSET] = makeLink(secondaryIndex);
    }

    targetBit = ONE << (wordSize - SHIFT_OFFSET - primaryIndex);
    secondary[secondaryIndex] = secondaryWord | targetBit;
}

void ChoiceDictionary::removeFromSecondary(uint primaryIndex) {
    uint targetBit;
    uint secondaryWord;
    uint newSecondaryWord;

    uint secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;

    secondaryWord = secondary[secondaryIndex];
    targetBit = ONE << (wordSize - SHIFT_OFFSET - primaryIndex);
    newSecondaryWord = secondaryWord & ~targetBit;
    secondary[secondaryIndex] = newSecondaryWord;

    if (newSecondaryWord == 0) breakLink(secondaryIndex);
}

uint ChoiceDictionary::makeLink(uint secondaryIndex) {
    uint validatorIndex;

    pointer++;

    validatorIndex = pointer - POINTER_OFFSET;
    validator[validatorIndex] = secondaryIndex + TUPEL_OFFSET;

    return validatorIndex;
}

void ChoiceDictionary::breakLink(uint secondaryIndex) {
    uint validatorIndex = secondary[secondaryIndex + TUPEL_OFFSET];
    secondary[secondaryIndex + TUPEL_OFFSET] = 0;
    validator[validatorIndex] = 0;
    shrinkValidator(validatorIndex);
}

void ChoiceDictionary::shrinkValidator(uint validatorIndex) {
    if (validatorIndex < pointer - 1) {
        uint secondaryTarget = validator[pointer - 1];
        validator[validatorIndex] = secondaryTarget;
        secondary[secondaryTarget] = validatorIndex;
    }
    pointer--;
}

bool ChoiceDictionary::isInitialized(uint primaryIndex) {
    uint secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;
    uint secondaryWord = secondary[secondaryIndex];
    uint targetBit = ONE << (wordSize - SHIFT_OFFSET - primaryIndex);

    return (secondaryWord & targetBit) != 0 && hasColor(primaryIndex) && pointer > 0;
}

bool ChoiceDictionary::hasColor(uint primaryIndex) {
    uint secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;
    uint link = secondary[secondaryIndex + TUPEL_OFFSET];

    if (link > wordCount / wordSize) {
        return false;
    } else if (validator[link] != secondaryIndex + TUPEL_OFFSET) {
        return false;
    } else {
        return true;
    }
}

