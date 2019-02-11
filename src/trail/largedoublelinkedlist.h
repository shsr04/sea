#ifndef SRC_TRAIL_LARGEDOUBLELINKEDLIST_H_
#define SRC_TRAIL_LARGEDOUBLELINKEDLIST_H_

#include "doublelinkedlist.h"
#include <vector>
#include <ostream>

namespace Sealib {
/**
 * Implementation of the interface provided by DoubleLinkedList for words of size 32bits.
 */
class LargeDoubleLinkedList : public DoubleLinkedList {
 private:
    std::vector<uint> links;
    uint current;

 public:
    ~LargeDoubleLinkedList() override = default;
    explicit LargeDoubleLinkedList(uint size);
    /**
     * @return the currently pointed to value and removes it, or (uint)-1 if empty.
     */
    uint get() override;

    /**
    * @param element to be removed
    * @return the removed element, or (uint)-1 if the element was not present.
    */
    uint remove(uint idx) override;
    /**
     * @return - true if empty, false otherwise
     */
    bool isEmpty() override;
};
}  // namespace Sealib
#endif  //  SRC_TRAIL_LARGEDOUBLELINKEDLIST_H_
