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
/**
 * Recognizes if a given biconnected graph is outerplanar.
 * (Note: You can use the BCC iterator to make a biconnected subgraph from an
 * arbitrary graph.)
 * EFFICIENCY: O(n*log(log(n))) time, O(n) bits
 *
 * @author Simon Heuser
 */
class OuterplanarChecker {
 public:
    /**
     * Create the outerplanarity checker.
     * @param g biconnected graph
     */
    explicit OuterplanarChecker(UndirectedGraph const& g);

    /**
     * Run the algorithm to check if the given biconnected graph G is
     * outerplanar.
     * @return true if G is outerplanar, false otherwise
     */
    bool isOuterplanar();

 private:
    VirtualGraph g;
    uint64_t n, m;
    std::vector<bool> tried;
    CompactArray paths;
    RankSelect pathOffset;

    bool removeClosedChains();
    bool removeAllChains();

    bool incrementPaths(uint64_t u, uint64_t k);

    /**
     * Data referring to a chain. A chain is a path where each vertex has degree
     * 2 except two endpoints which have degree > 2.
     */
    struct ChainData {
        std::pair<uint64_t, uint64_t>
            c1,  ///< endpoint u1 + outgoing edge k1 pointing into the chain
            c2;  ///< endpoint u2 + outgoing edge k2 pointing into the chain
        bool isClosed =
                 false,      ///< true if the endpoints are connected by an edge
            isGood = false,  ///< true if one of the endpoints has degree < 4
            isCycle = false;  ///< true if the chain has only degree 2 vertices
    };

    /**
     * Walk through the chain that contains u.
     * @param u Vertex in the interior of the chain
     * @return data collected when iterating over the chain
     */
    ChainData chain(uint64_t u);

    void forEach(ChainData const& c, Consumer v,
                 BiConsumer e = [](uint64_t, uint64_t) {});
};
}  // namespace Sealib

#endif  // SEALIB_ITERATOR_OUTERPLANARCHECKER_H_
