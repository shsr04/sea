#ifndef SEALIB_ITERATOR_OUTERPLANARCHECKER_H_
#define SEALIB_ITERATOR_OUTERPLANARCHECKER_H_
#include <functional>
#include <set>
#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/staticspacestorage.h"
#include "sealib/dictionary/choicedictionary.h"
#include "sealib/graph/undirectedgraph.h"
#include "sealib/graph/virtualgraph.h"

namespace Sealib {
class OuterplanarChecker {
 public:
    explicit OuterplanarChecker(UndirectedGraph const &g);

    bool isOuterplanar();

 private:
    VirtualGraph g;
    uint64_t n, m;
    ChoiceDictionary present;
    ChoiceDictionary degreeTwo;
    std::vector<std::set<uint64_t>> virtualEdges;
    ChoiceDictionary removedPaths;

    bool isGood(std::pair<uint64_t, uint64_t> p) const {
        return g.deg(p.first) <= 4 || g.deg(p.second) <= 4;
    }
    bool isClosed(std::pair<uint64_t, uint64_t> p) const {
        uint64_t u = p.first, v = p.second;
        if (g.deg(u) > g.deg(v)) {
            u = p.second;
            v = p.first;
        }
        for (uint64_t a = 0; a < g.deg(u); a++) {
            if (g.head(u, a) == v) return true;
        }
        return false;
    }

    /**
     * Walk through the chain that contains u.
     * @param u Vertex in the interior of the chain
     * @param action Optional action to perform for each vertex on the chain
     * @return {E,P} Pair containing two pairs: 1) the endpoints of the chain,
     * 2) the vertices in the interior adjacent to the respective endpoint
     */
    std::pair<std::pair<uint64_t, uint64_t>, std::pair<uint64_t, uint64_t>> chain(
        uint64_t u, Consumer action = [](uint64_t) {});
};
}  // namespace Sealib

#endif  // SEALIB_ITERATOR_OUTERPLANARCHECKER_H_
