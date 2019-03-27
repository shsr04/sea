#ifndef SRC_MARKER_SIMPLECUTVERTEXITERATOR_H_
#define SRC_MARKER_SIMPLECUTVERTEXITERATOR_H_
#include <stack>
#include <vector>
#include "sealib/graph/undirectedgraph.h"
#include "sealib/iterator/iterator.h"

namespace Sealib {
class SimpleCutVertexIterator : Iterator<uint64_t> {
 public:
    explicit SimpleCutVertexIterator(UndirectedGraph const &graph)
        : g(graph), n(g.getOrder()), number(n, INVALID), lowpt(n), parent(n) {}

    void init() override;

    bool more() override;

    uint64_t next() override;

 private:
    UndirectedGraph const &g;
    uint64_t n;
    std::vector<uint64_t> number;
    std::vector<uint64_t> lowpt;
    std::vector<uint64_t> parent;
    std::stack<std::pair<uint64_t, uint64_t>> s;
    std::deque<uint64_t> cut;
    uint64_t i = 0;

    void findLowpt(uint64_t u0);
};
}  // namespace Sealib

#endif  // SRC_MARKER_SIMPLECUTVERTEXITERATOR_H_
