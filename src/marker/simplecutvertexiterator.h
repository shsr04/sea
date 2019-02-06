#ifndef SRC_MARKER_SIMPLECUTVERTEXITERATOR_H_
#define SRC_MARKER_SIMPLECUTVERTEXITERATOR_H_
#include <limits>
#include <set>
#include <vector>
#include "sealib/iterator/dfs.h"
#include "sealib/iterator/iterator.h"

namespace Sealib {
class SimpleCutVertexIterator : Iterator<uint> {
 public:
    explicit SimpleCutVertexIterator(UndirectedGraph const *graph)
        : g(graph),
          n(g->getOrder()),
          color(n),
          number(n, std::numeric_limits<uint>::max()),
          lowpt(n) {}

    void init() override;

    bool more() override;

    uint next() override;

    uint64_t byteSize() const {
        return color.capacity() * sizeof(uint8_t) +
               s.capacity() * sizeof(std::pair<uint, uint>) +
               (number.capacity() + lowpt.capacity() + tree.capacity()) *
                   sizeof(uint) +
               cut.size() * sizeof(uint);
    }

 private:
    UndirectedGraph const *g;
    uint n;
    std::vector<uint8_t> color;
    std::vector<std::pair<uint, uint>> s;
    std::vector<uint> number;
    std::vector<uint> lowpt;
    std::vector<uint> tree;
    std::set<uint> cut;
    std::set<uint>::iterator cutI;
};
}  // namespace Sealib

#endif  // SRC_MARKER_SIMPLECUTVERTEXITERATOR_H_
