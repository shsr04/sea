#ifndef SRC_PLANAR_SIMPLEOUTERPLANARCHECKER_H_
#define SRC_PLANAR_SIMPLEOUTERPLANARCHECKER_H_
#include <queue>
#include <vector>
#include "sealib/_types.h"
#include "sealib/graph/undirectedgraph.h"

namespace Sealib {
/**
 * Simple implementation that checks if a biconnected graph is outerplanar. It
 * works by repeatedly removing vertices of degree 2. If necessary, the
 * neighbours of the removed vertex are connected using a virtual edge.
 * (Wiegers; 1986)
 */
class SimpleOuterplanarChecker {
 public:
    /**
     * Create a new object that checks the given biconnected graph for
     * outerplanarity.
     * @param g biconnected undirected graph G=(V,E)
     */
    explicit SimpleOuterplanarChecker(UndirectedGraph const &g);

    /**
     * Run the check for outerplanarity.
     * @return true if the given graph is outerplanar, false otherwise
     */
    bool isOuterplanar();

 private:
    UndirectedGraph const &g;
    uint64_t n, m;
    std::vector<bool> present;
    std::queue<uint64_t> degTwo;
    std::vector<std::vector<uint64_t>> virtualAdj;
    std::vector<std::vector<bool>> isOuter;

    uint64_t actualDeg(uint64_t u);
    uint64_t actualHead(uint64_t u,uint64_t k);
    bool edgeBetween(uint64_t u, uint64_t v);
    uint64_t outerIndex(uint64_t u, uint64_t v);
    void connectNeighbours(uint64_t u);
};
}  // namespace Sealib
#endif  // SRC_PLANAR_SIMPLEOUTERPLANARCHECKER_H_
