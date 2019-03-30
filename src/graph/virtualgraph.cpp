#include "sealib/graph/virtualgraph.h"

namespace Sealib {

VirtualGraph::VirtualGraph(Graph const &graph)
    : g(graph), n(g.getOrder()), presentVertices(n), presentEdges(n) {}

uint64_t VirtualGraph::deg(uint64_t u) const {
    uint64_t r = 0;
    for (bool p : presentEdges[u]) {
        r += p;
    }
    return r;
}

uint64_t VirtualGraph::head(uint64_t u, uint64_t k) const {
    uint64_t b = 0;
    std::vector<bool> adj = presentEdges[u];
    for (uint64_t a = 0; a < g.deg(u); a++) {
        if (adj[a] == 1) {
            if (b == k) {
                return g.head(u, a);
            }
            b++;
        }
    }
    return INVALID;
}

void VirtualGraph::removeVertex(uint64_t u) { presentVertices[u] = 0; }

void VirtualGraph::removeEdge(uint64_t u, uint64_t v) {
    for (uint64_t a = 0; a < g.deg(u); a++) {
        if (g.head(u, a) == v) {
            presentEdges[u][a] = 0;
            break;
        }
    }
}

uint64_t VirtualGraph::getOrder() const { return n; }

}  // namespace Sealib
