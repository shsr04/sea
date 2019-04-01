#ifndef SEALIB_ITERATOR_OUTERPLANARCHECKER_H_
#define SEALIB_ITERATOR_OUTERPLANARCHECKER_H_
#include <set>
#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/compactarray.h"
#include "sealib/dictionary/raggeddictionary.h"
#include "sealib/dictionary/rankselect.h"
#include "sealib/graph/undirectedgraph.h"
#include "sealib/graph/virtualgraph.h"

namespace Sealib {
class OuterplanarChecker {
 public:
    explicit OuterplanarChecker(UndirectedGraph const& g);

    bool isOuterplanar();

 private:
    VirtualGraph g;
    uint64_t n, m;
    CompactArray paths;
    RankSelect pathOffset;
    RaggedDictionary shortcuts;

    bool removeClosedChains();
    bool removeAllChains();

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

    struct ChainData {
        std::pair<uint64_t, uint64_t> c1, c2;
        bool isClosed = false, isGood = false, isCycle = false;
    };

    /**
     * Walk through the chain that contains u.
     * @param u Vertex in the interior of the chain
     * @return data collected when iterating over the chain
     */
    ChainData chain(uint64_t u);

    void forEach(ChainData const& c, Consumer f);
};
}  // namespace Sealib

#endif  // SEALIB_ITERATOR_OUTERPLANARCHECKER_H_
