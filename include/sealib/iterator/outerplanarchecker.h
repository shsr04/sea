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
