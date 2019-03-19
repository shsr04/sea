#include "sealib/graph/virtualgraph.h"
#include "sealib/_types.h"

namespace Sealib {

static std::vector<std::vector<bool>> makeBits(Graph const &g) {
    std::vector<std::vector<bool>> v;
    for (uint a = 0; a < g.getOrder(); a++) {
        v.emplace_back(std::vector<bool>(g.deg(a)));
    }
    return v;
}

VirtualGraph::VirtualGraph(Graph const &graph)
    : g(graph),
      n(g.getOrder()),
      actualDegrees(g),
      deletedAdjacencies(makeBits(g)) {
    for (uint a = 0; a < n; a++) {
        actualDegrees.insert(a, g.deg(a));
    }
}

uint64_t VirtualGraph::deg(uint64_t u) const {
    return actualDegrees.get(u);
}

uint64_t VirtualGraph::head(uint64_t u, uint64_t k) const {
    uint64_t b = 0;
    std::vector<bool> adj = deletedAdjacencies[u];
    for (uint64_t a = 0; a < g.deg(u); a++) {
        if (adj[a] == 0) {
            if (b == k) {
                return g.head(u, a);
            }
            b++;
        }
    }
    return INVALID;
}

void VirtualGraph::removeEdge(uint64_t u, uint64_t v) {
    uint64_t k = INVALID;
    for (uint64_t a = 0; a < g.deg(u); a++) {
        if (g.head(u, a) == v) {
            k = a;
            break;
        }
    }
    deletedAdjacencies[u][k] = 1;
    actualDegrees.insert(u, deg(u) - 1UL);
}

uint64_t VirtualGraph::getOrder() const { return n; }

}  // namespace Sealib
