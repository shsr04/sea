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
          number(n, std::numeric_limits<uint>::max()),
          lowpt(n) {}

    void init() override;

    bool more() override;

    uint next() override;

    uint64_t byteSize() const {
        return (number.capacity()+lowpt.capacity()+tree.capacity())*sizeof(uint)+cut.size();
    }

 private:
    UndirectedGraph const *g;
    uint n;
    std::vector<uint> number;
    std::vector<uint> lowpt;
    std::vector<uint> tree;
    std::set<uint> cut;
    std::set<uint>::iterator cutI;
};
}  // namespace Sealib

#endif  // SRC_MARKER_SIMPLECUTVERTEXITERATOR_H_
