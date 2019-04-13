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
 * Recognizes if a given biconnected input graph is outerplanar.
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
     * Run the algorithm to check if the given input graph is outerplanar.
     * @return true if G is outerplanar, false otherwise
     */
    bool isOuterplanar();

 private:
    VirtualGraph g;
    uint64_t n, m;
    std::vector<bool> tried;
    CompactArray paths;
    RankSelect pathOffset;
    RaggedDictionary shortcuts;

    bool removeClosedChains();
    bool removeAllChains();

    bool incrementPaths(uint64_t u, uint64_t k);

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

    void forEach(ChainData const& c, BiConsumer f);
};
}  // namespace Sealib

#endif  // SEALIB_ITERATOR_OUTERPLANARCHECKER_H_
