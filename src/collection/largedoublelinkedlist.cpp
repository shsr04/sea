#include "../trail/largedoublelinkedlist.h"
#include <iostream>

Sealib::LargeDoubleLinkedList::LargeDoubleLinkedList(uint size) :
    links(size * 2, 1),
    current(size == 0 ? (uint) -1 : 0) {
}

uint Sealib::LargeDoubleLinkedList::get() {
    if (current != (uint) -1) {
        uint retVal = current;
        remove(current);
        return retVal;
    } else {
        return (uint) -1;
    }
}

uint Sealib::LargeDoubleLinkedList::remove(uint idx) {
    if (current == (uint) -1) {  // empty list
        return current;
    }

    auto actualIdx = static_cast<int32_t>(idx * 2);

    int32_t prevIdx = actualIdx;

    prevIdx -= static_cast<int32_t>(links[static_cast<uint >(actualIdx)] * 2);

    auto n = static_cast<int32_t>(links.size());
    int32_t mod = prevIdx % n;
    if (prevIdx < 0) {
        prevIdx = n + mod;
    } else {
        prevIdx = mod;
    }
    if (prevIdx == n) {
        prevIdx = 0;
    }
    if (prevIdx == actualIdx) {  // last element
        current = (uint) -1;
        return idx;
    }
    links[static_cast<uint >(prevIdx) + 1] += links[static_cast<uint >(actualIdx) + 1];

    int32_t nextIdx = actualIdx;
    nextIdx += static_cast<int32_t>(links[static_cast<uint >(actualIdx) + 1] * 2);
    nextIdx = nextIdx % n;
    links[static_cast<uint >(nextIdx)] += links[static_cast<uint >(actualIdx)];

    current = static_cast<uint>(nextIdx / 2);
    return current;
}

bool Sealib::LargeDoubleLinkedList::isEmpty() {
    return current == (uint) -1;
}
