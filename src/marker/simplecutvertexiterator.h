#ifndef SEALIB_MARKER_SIMPLECUTVERTEXITERATOR_H_
#define SEALIB_MARKER_SIMPLECUTVERTEXITERATOR_H_
#include <set>
#include <vector>
#include "sealib/iterator/dfs.h"
#include "sealib/iterator/iterator.h"

namespace Sealib {
class SimpleCutVertexIterator : Iterator<uint> {
 public:
    SimpleCutVertexIterator(UndirectedGraph const *graph)
        : g(graph), n(g->getOrder()), number(n), lowpt(n) {}

    void init() override;

    bool more() override;

    uint next() override;

 private:
    UndirectedGraph const *g;
    uint n;
    std::vector<uint> number;
    std::vector<uint> lowpt;
    std::set<uint> cut;
    std::set<uint>::iterator cutI;
};
}  // namespace Sealib

#endif  // SEALIB_MARKER_SIMPLECUTVERTEXITERATOR_H_
